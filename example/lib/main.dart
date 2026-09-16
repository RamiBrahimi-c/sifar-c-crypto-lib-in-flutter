import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:image_picker/image_picker.dart';
import 'package:path_provider/path_provider.dart';
import 'package:my_native_wrapper/sifar.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});
  @override
  Widget build(BuildContext context) => MaterialApp(
    title: 'Sifar',
    theme: ThemeData(useMaterial3: true, colorSchemeSeed: Colors.indigo),
    home: const HomePage(),
  );
}

class HomePage extends StatefulWidget {
  const HomePage({super.key});
  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final _plainCtrl = TextEditingController(text: 'hello world!!!!!');
  final _keyCtrl   = TextEditingController(text: '00112233445566778899aabbccddeeff');

  String _result = '';
  String? _encryptedImagePath;
  String? _decryptedImagePath;
  bool _busy = false;

  @override
  void dispose() {
    _plainCtrl.dispose();
    _keyCtrl.dispose();
    super.dispose();
  }

  Uint8List? _hexToBytes(String hex) {
    hex = hex.replaceAll(RegExp(r'\s'), '');
    if (hex.length.isOdd) return null;
    try {
      return Uint8List.fromList(List.generate(
        hex.length ~/ 2,
        (i) => int.parse(hex.substring(i * 2, i * 2 + 2), radix: 16),
      ));
    } catch (_) {
      return null;
    }
  }

  String _bytesToHex(Uint8List b) =>
      b.map((x) => x.toRadixString(16).padLeft(2, '0')).join();

  Uint8List _requireKey() {
    final key = _hexToBytes(_keyCtrl.text);
    if (key == null ||
        (key.length != 16 && key.length != 24 && key.length != 32)) {
      throw ArgumentError('Key must be 16/24/32 bytes of hex');
    }
    return key;
  }

  // ----------------------------------------------------------------
  // Text encrypt / decrypt
  // ----------------------------------------------------------------
  void _run() {
    setState(() => _result = '');
    try {
      final key = _requireKey();
      final plain = Uint8List.fromList(utf8.encode(_plainCtrl.text));
      if (plain.length % 16 != 0) {
        throw ArgumentError(
            'Plaintext must be a multiple of 16 bytes (${plain.length} given)');
      }

      final aes = SifarAes(key);
      final enc = aes.encrypt(plain);
      final dec = aes.decrypt(enc);
      aes.dispose();

      setState(() {
        _result = 'CIPHER:\n${_bytesToHex(enc)}\n\n'
            'DECRYPTED:\n${utf8.decode(dec)}';
      });
    } catch (e) {
      setState(() => _result = 'ERROR: $e');
    }
  }

  // ----------------------------------------------------------------
  // Image encrypt
  // ----------------------------------------------------------------
  Future<void> _pickAndEncrypt() async {
    setState(() {
      _busy = true;
      _encryptedImagePath = null;
      _decryptedImagePath = null;
    });
    try {
      final picked = await ImagePicker().pickImage(source: ImageSource.gallery);
      if (picked == null) {
        setState(() => _busy = false);
        return;
      }

      final dir = await getApplicationDocumentsDirectory();
      final stamp = DateTime.now().millisecondsSinceEpoch;
      final inputPath  = '${dir.path}/input_$stamp.png';
      final outputPath = '${dir.path}/encrypted_$stamp.png';

      await File(picked.path).copy(inputPath);

      SifarImage.encryptImage(
        cipher: 'aes',
        inputPath: inputPath,
        outputPath: outputPath,
        key: _requireKey(),
      );

      setState(() {
        _encryptedImagePath = outputPath;
        _busy = false;
      });
    } catch (e) {
      setState(() {
        _result = 'IMAGE ERROR: $e';
        _busy = false;
      });
    }
  }

  // ----------------------------------------------------------------
  // Image decrypt
  // ----------------------------------------------------------------
  Future<void> _pickAndDecrypt() async {
    setState(() {
      _busy = true;
      _encryptedImagePath = null;
      _decryptedImagePath = null;
    });
    try {
      final picked = await ImagePicker().pickImage(source: ImageSource.gallery);
      if (picked == null) {
        setState(() => _busy = false);
        return;
      }

      final dir = await getApplicationDocumentsDirectory();
      final stamp = DateTime.now().millisecondsSinceEpoch;
      final inputPath  = '${dir.path}/input_$stamp.png';
      final outputPath = '${dir.path}/decrypted_$stamp.png';

      await File(picked.path).copy(inputPath);

      SifarImage.decryptImage(
        cipher: 'aes',
        inputPath: inputPath,
        outputPath: outputPath,
        key: _requireKey(),
      );

      setState(() {
        _decryptedImagePath = outputPath;
        _busy = false;
      });
    } catch (e) {
      setState(() {
        _result = 'IMAGE ERROR: $e';
        _busy = false;
      });
    }
  }

  Future<void> _roundTrip() async {
    setState(() {
      _busy = true;
      _encryptedImagePath = null;
      _decryptedImagePath = null;
      _result = '';
    });
    try {
      final picked = await ImagePicker().pickImage(source: ImageSource.gallery);
      if (picked == null) {
        setState(() => _busy = false);
        return;
      }

      final dir = await getApplicationDocumentsDirectory();
      final stamp = DateTime.now().millisecondsSinceEpoch;
      final inputPath  = '${dir.path}/input_$stamp.png';
      final encPath    = '${dir.path}/encrypted_$stamp.png';
      final decPath    = '${dir.path}/decrypted_$stamp.png';

      await File(picked.path).copy(inputPath);

      final key = _requireKey();

      SifarImage.encryptImage(
        cipher: 'aes',
        inputPath: inputPath,
        outputPath: encPath,
        key: key,
      );

      SifarImage.decryptImage(
        cipher: 'aes',
        inputPath: encPath,
        outputPath: decPath,
        key: key,
      );

      // Compare decrypted vs original byte-for-byte
      final origBytes = await File(inputPath).readAsBytes();
      final decBytes  = await File(decPath).readAsBytes();
      final match = _bytesEqual(origBytes, decBytes);

      setState(() {
        _encryptedImagePath = encPath;
        _decryptedImagePath = decPath;
        _result = match
            ? '✅ Round-trip OK (${origBytes.length} bytes match)'
            : '❌ Mismatch: orig=${origBytes.length} dec=${decBytes.length}';
        _busy = false;
      });
    } catch (e) {
      setState(() {
        _result = 'ROUND-TRIP ERROR: $e';
        _busy = false;
      });
    }
  }
    
  bool _bytesEqual(Uint8List a, Uint8List b) {
    if (a.length != b.length) return false;
    for (var i = 0; i < a.length; i++) {
      if (a[i] != b[i]) return false;
    }
    return true;
  }

  // ----------------------------------------------------------------
  // Middle panel — shows image or text
  // ----------------------------------------------------------------

  Widget _buildResultPanel(BuildContext context) {
    if (_encryptedImagePath != null) {
      return Column(
        children: [
          const Text('ENCRYPTED', style: TextStyle(fontWeight: FontWeight.bold)),
          const SizedBox(height: 8),
          Image.file(File(_encryptedImagePath!)),
          const SizedBox(height: 8),
          Text('Saved: $_encryptedImagePath',
              style: const TextStyle(fontSize: 11)),
          if (_decryptedImagePath != null) ...[
            const SizedBox(height: 24),
            const Text('DECRYPTED',
                style: TextStyle(fontWeight: FontWeight.bold)),
            const SizedBox(height: 8),
            Image.file(File(_decryptedImagePath!)),
            const SizedBox(height: 8),
            Text('Saved: $_decryptedImagePath',
                style: const TextStyle(fontSize: 11)),
          ],
          const SizedBox(height: 16),
          Text(
            _result,
            style: const TextStyle(fontFamily: 'monospace', fontSize: 13),
            textAlign: TextAlign.center,
          ),
        ],
      );
    }

    return SelectableText(
      _result.isEmpty ? '(result will appear here)' : _result,
      style: const TextStyle(fontFamily: 'monospace', fontSize: 13),
    );
  }

  // ----------------------------------------------------------------
  // Build
  // ----------------------------------------------------------------
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Sifar')),
      body: SafeArea(
        child: Padding(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              TextField(
                controller: _plainCtrl,
                maxLines: 3,
                decoration: const InputDecoration(
                  labelText: 'Plaintext',
                  border: OutlineInputBorder(),
                ),
              ),
              const SizedBox(height: 12),
              TextField(
                controller: _keyCtrl,
                decoration: const InputDecoration(
                  labelText: 'Key (hex)',
                  helperText:
                      '32 hex chars = AES-128, 48 = AES-192, 64 = AES-256',
                  border: OutlineInputBorder(),
                ),
              ),
              const SizedBox(height: 16),
              Expanded(
                child: Container(
                  padding: const EdgeInsets.all(12),
                  decoration: BoxDecoration(
                    color:
                        Theme.of(context).colorScheme.surfaceContainerHighest,
                    borderRadius: BorderRadius.circular(8),
                  ),
                  child: SingleChildScrollView(
                    child: _buildResultPanel(context),
                  ),
                ),
              ),
              const SizedBox(height: 16),
              FilledButton(
                onPressed: _busy ? null : _run,
                style: FilledButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 16),
                ),
                child: const Text('Encrypt Text'),
              ),
              const SizedBox(height: 8),
              FilledButton.tonal(
                onPressed: _busy ? null : _pickAndEncrypt,
                style: FilledButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 16),
                ),
                child: Text(_busy ? 'Working…' : 'Encrypt Image'),
              ),
              const SizedBox(height: 8),
              FilledButton.tonal(
                onPressed: _busy ? null : _pickAndDecrypt,
                style: FilledButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 16),
                ),
                child: Text(_busy ? 'Working…' : 'Decrypt Image'),
              ),
              const SizedBox(height: 8),
              FilledButton.tonal(
                onPressed: _busy ? null : _roundTrip,
                style: FilledButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 16),
                ),
                child: Text(_busy ? 'Working…' : 'Round-Trip Test'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}