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
    } catch (_) { return null; }
  }

  String _bytesToHex(Uint8List b) =>
      b.map((x) => x.toRadixString(16).padLeft(2, '0')).join();

  void _run() {
    setState(() => _result = '');
    try {
      final key = _hexToBytes(_keyCtrl.text);
      if (key == null || (key.length != 16 && key.length != 24 && key.length != 32)) {
        throw ArgumentError('Key must be 16/24/32 bytes of hex');
      }

      final plain = Uint8List.fromList(utf8.encode(_plainCtrl.text));
      if (plain.length % 16 != 0) {
        throw ArgumentError('Plaintext must be a multiple of 16 bytes (${plain.length} given)');
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

  Future<void> _pickAndEncrypt() async {
    setState(() {
      _busy = true;
      _encryptedImagePath = null;
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

      final key = _hexToBytes(_keyCtrl.text);
      if (key == null) throw ArgumentError('Bad key');

      SifarImage.encryptImage(
        cipher: 'aes',
        inputPath: inputPath,
        outputPath: outputPath,
        key: key,
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
                  helperText: '32 hex chars = AES-128, 48 = AES-192, 64 = AES-256',
                  border: OutlineInputBorder(),
                ),
              ),
              const SizedBox(height: 16),
              Expanded(
                child: Container(
                  padding: const EdgeInsets.all(12),
                  decoration: BoxDecoration(
                    color: Theme.of(context).colorScheme.surfaceContainerHighest,
                    borderRadius: BorderRadius.circular(8),
                  ),
                  child: SingleChildScrollView(
                    child: _encryptedImagePath != null
                        ? Column(
                            children: [
                              Image.file(File(_encryptedImagePath!)),
                              const SizedBox(height: 8),
                              Text('Saved: $_encryptedImagePath',
                                  style: const TextStyle(fontSize: 11)),
                            ],
                          )
                        : SelectableText(
                            _result.isEmpty ? '(result will appear here)' : _result,
                            style: const TextStyle(fontFamily: 'monospace', fontSize: 13),
                          ),
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
            ],
          ),
        ),
      ),
    );
  }
}