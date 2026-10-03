import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:image_picker/image_picker.dart';
import 'package:path_provider/path_provider.dart';
import 'package:my_native_wrapper/sifar.dart';
import 'dart:math';
import 'package:flutter/services.dart';
import 'about_page.dart';
import 'package:gal/gal.dart';


void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});
  @override
  Widget build(BuildContext context) => MaterialApp(
    title: 'Sifar',
    themeMode: ThemeMode.system,
    theme: ThemeData(
      useMaterial3: true,
      colorSchemeSeed: Colors.indigo,
      brightness: Brightness.light,
    ),
    darkTheme: ThemeData(
      useMaterial3: true,
      colorSchemeSeed: Colors.indigo,
      brightness: Brightness.dark,
    ),
    home: const HomePage(),
  );
}

const _ciphers = ['aes', 'des', '3des', 'blowfish', 'tea', 'xtea', 'rc4', 'redpike'];

class HomePage extends StatefulWidget {
  const HomePage({super.key});
  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> with SingleTickerProviderStateMixin {
  late final TabController _tabs;

  final _keyCtrl = TextEditingController(text: '00112233445566778899aabbccddeeff');
  final _ivCtrl  = TextEditingController(text: '00000000000000000000000000000000');
  String _cipher = 'aes';
  String _mode   = 'ecb';

  static const _streamCiphers = {'rc4'};
  bool get _cipherSupportsModes => !_streamCiphers.contains(_cipher);
  bool get _modeNeedsIv        => _cipherSupportsModes && _mode != 'ecb';

  int _lastTab = 0;

  @override
  void initState() {
    super.initState();
    _tabs = TabController(length: 3, vsync: this);
    _tabs.addListener(_onTabChanged);
    _keyCtrl.addListener(_onKeyChanged);
    _ivCtrl.addListener(_onIvChanged);
  }

  @override
  void dispose() {
    _tabs.removeListener(_onTabChanged);
    _keyCtrl.removeListener(_onKeyChanged);
    _ivCtrl.removeListener(_onIvChanged);
    _tabs.dispose();
    _keyCtrl.dispose();
    _ivCtrl.dispose();
    super.dispose();
  }

  void _onIvChanged()  => setState(() {});
  void _onKeyChanged() => setState(() {});

  void _onTabChanged() {
    if (_tabs.index != _lastTab) {
      _lastTab = _tabs.index;
      setState(() {});
    }
  }

  bool get _showCipherControls => _tabs.index != 2;

  bool get _keyIsValid {
    final key = _hexToBytes(_keyCtrl.text);
    if (key == null) return false;
    final expected = keySizeOf(_cipher);
    if (expected > 0) return key.length == expected;
    return key.isNotEmpty;
  }

  bool get _ivIsValid {
    final iv = _hexToBytes(_ivCtrl.text);
    if (iv == null) return false;
    final expected = blockSizeOf(_cipher);
    return expected > 0 && iv.length == expected;
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

  String _keyHint() {
    switch (_cipher) {
      case 'aes':      return '16, 24, or 32 bytes → 32, 48, or 64 hex chars';
      case 'des':      return '8 bytes → 16 hex chars';
      case '3des':     return '24 bytes → 48 hex chars';
      case 'blowfish': return 'variable 1–56 bytes';
      case 'rc4':      return 'variable 1–256 bytes';
      case 'tea':      return '16 bytes → 32 hex chars';
      case 'xtea':     return '16 bytes → 32 hex chars';
      case 'redpike':  return '8 bytes → 16 hex chars';
      default:         return '';
    }
  }

  Uint8List _requireKey() {
    final key = _hexToBytes(_keyCtrl.text);
    if (key == null) throw ArgumentError('Key must be valid hex');
    final expected = keySizeOf(_cipher);
    if (expected > 0 && key.length != expected) {
      throw ArgumentError(
        '${_cipher.toUpperCase()} requires $expected bytes (${expected * 2} hex chars), '
        'got ${key.length}',
      );
    }
    return key;
  }

  Uint8List _requireIv() {
    final iv = _hexToBytes(_ivCtrl.text);
    if (iv == null) throw ArgumentError('IV must be valid hex');
    final expected = blockSizeOf(_cipher);
    if (iv.length != expected) {
      throw ArgumentError('IV must be $expected bytes (${expected * 2} hex chars)');
    }
    return iv;
  }

  void _generateIv() {
    final len = blockSizeOf(_cipher);
    if (len <= 0) return;
    final rng = Random.secure();
    final iv = Uint8List.fromList(List.generate(len, (_) => rng.nextInt(256)));
    _ivCtrl.text = iv.map((b) => b.toRadixString(16).padLeft(2, '0')).join();
  }

  void _generateKey() {
    final expected = keySizeOf(_cipher);
    final length = expected > 0 ? expected : 16;
    final rng = Random.secure();
    final key = Uint8List.fromList(List.generate(length, (_) => rng.nextInt(256)));
    _keyCtrl.text = key.map((b) => b.toRadixString(16).padLeft(2, '0')).join();
  }

  Widget _buildKeyField(BuildContext context) {
    return Row(
      children: [
        Expanded(
          child: TextField(
            controller: _keyCtrl,
            decoration: InputDecoration(
              labelText: 'Key (hex)',
              helperText: _keyHint(),
              border: const OutlineInputBorder(),
              enabledBorder: OutlineInputBorder(
                borderSide: BorderSide(
                  color: _keyCtrl.text.isEmpty
                      ? Theme.of(context).colorScheme.outline
                      : (_keyIsValid ? Colors.green : Colors.red),
                  width: 2,
                ),
              ),
              focusedBorder: OutlineInputBorder(
                borderSide: BorderSide(
                  color: _keyCtrl.text.isEmpty
                      ? Theme.of(context).colorScheme.primary
                      : (_keyIsValid ? Colors.green : Colors.red),
                  width: 2,
                ),
              ),
              suffixIcon: _keyCtrl.text.isEmpty
                  ? null
                  : IconButton(
                      icon: const Icon(Icons.copy, size: 18),
                      tooltip: 'Copy key',
                      onPressed: () {
                        Clipboard.setData(ClipboardData(text: _keyCtrl.text));
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(content: Text('Key copied')),
                        );
                      },
                    ),
            ),
          ),
        ),
        const SizedBox(width: 8),
        IconButton(
          onPressed: _generateKey,
          icon: const Icon(Icons.casino),
          tooltip: 'Random key',
        ),
      ],
    );
  }

  Widget _buildIvField(BuildContext context) {
    return Row(
      children: [
        Expanded(
          child: TextField(
            controller: _ivCtrl,
            decoration: InputDecoration(
              labelText: 'IV (hex)',
              helperText: 'must be ${blockSizeOf(_cipher) * 2} hex chars',
              border: const OutlineInputBorder(),
              enabledBorder: OutlineInputBorder(
                borderSide: BorderSide(
                  color: _ivCtrl.text.isEmpty
                      ? Theme.of(context).colorScheme.outline
                      : (_ivIsValid ? Colors.green : Colors.red),
                  width: 2,
                ),
              ),
              focusedBorder: OutlineInputBorder(
                borderSide: BorderSide(
                  color: _ivCtrl.text.isEmpty
                      ? Theme.of(context).colorScheme.primary
                      : (_ivIsValid ? Colors.green : Colors.red),
                  width: 2,
                ),
              ),
              suffixIcon: _ivCtrl.text.isEmpty
                  ? null
                  : IconButton(
                      icon: const Icon(Icons.copy, size: 18),
                      tooltip: 'Copy IV',
                      onPressed: () {
                        Clipboard.setData(ClipboardData(text: _ivCtrl.text));
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(content: Text('IV copied')),
                        );
                      },
                    ),
            ),
          ),
        ),
        const SizedBox(width: 8),
        IconButton(
          onPressed: _generateIv,
          icon: const Icon(Icons.casino),
          tooltip: 'Random IV',
        ),
      ],
    );
  }

  Widget _buildCipherDropdown(BuildContext context) {
    return Row(
      children: [
        const Text('Cipher: '),
        const SizedBox(width: 8),
        Expanded(
          child: DropdownButton<String>(
            isExpanded: true,
            value: _cipher,
            onChanged: (v) => setState(() {
              _cipher = v!;
              if (_streamCiphers.contains(_cipher)) _mode = 'ecb';
            }),
            items: _ciphers
                .map((c) => DropdownMenuItem(
                      value: c,
                      child: Text(c.toUpperCase()),
                    ))
                .toList(),
          ),
        ),
      ],
    );
  }

  Widget _buildModeDropdown(BuildContext context) {
    return Row(
      children: [
        const Text('Mode: '),
        const SizedBox(width: 8),
        Expanded(
          child: DropdownButton<String>(
            isExpanded: true,
            value: _mode,
            onChanged: (v) => setState(() => _mode = v!),
            items: const [
              DropdownMenuItem(value: 'ecb', child: Text('ECB')),
              DropdownMenuItem(value: 'cbc', child: Text('CBC')),
              DropdownMenuItem(value: 'cfb', child: Text('CFB')),
              DropdownMenuItem(value: 'ofb', child: Text('OFB')),
              DropdownMenuItem(value: 'ctr', child: Text('CTR')),
            ],
          ),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sifar'),
        actions: [
          IconButton(
            icon: const Icon(Icons.info_outline),
            tooltip: 'About',
            onPressed: () => Navigator.of(context).push(
              MaterialPageRoute(builder: (_) => const AboutPage()),
            ),
          ),
        ],
        bottom: TabBar(
          controller: _tabs,
          tabs: const [Tab(text: 'Text'), Tab(text: 'Image'), Tab(text: 'Hash')],
        ),
      ),
      body: Column(
        children: [
          if (_showCipherControls)
            Padding(
              padding: const EdgeInsets.fromLTRB(16, 16, 16, 8),
              child: Column(
                children: [
                  _buildKeyField(context),
                  if (_modeNeedsIv) ...[
                    const SizedBox(height: 12),
                    _buildIvField(context),
                  ],
                  const SizedBox(height: 12),
                  _buildCipherDropdown(context),
                  if (_cipherSupportsModes) ...[
                    const SizedBox(height: 8),
                    _buildModeDropdown(context),
                  ],
                ],
              ),
            ),
          Expanded(
            child: TabBarView(
              controller: _tabs,
              children: [
                _TextTab(
                  cipher: _cipher,
                  mode: _mode,
                  keyText: _keyCtrl.text,
                  ivText: _ivCtrl.text,
                  requireKey: _requireKey,
                  requireIv: _requireIv,
                  hexToBytes: _hexToBytes,
                ),
                _ImageTab(
                  cipher: _cipher,
                  mode: _mode,
                  requireKey: _requireKey,
                ),
                const _HashTab(),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

// ============================================================
// TEXT TAB
// ============================================================
class _TextTab extends StatefulWidget {
  final String cipher;
  final String mode;
  final String keyText;
  final String ivText;
  final Uint8List Function() requireKey;
  final Uint8List Function() requireIv;
  final Uint8List? Function(String) hexToBytes;

  const _TextTab({
    required this.cipher,
    required this.mode,
    required this.keyText,
    required this.ivText,
    required this.requireKey,
    required this.requireIv,
    required this.hexToBytes,
  });

  @override
  State<_TextTab> createState() => _TextTabState();
}

enum CipherView { hex, latin1 }
enum PlainFormat { text, hex }

class _TextTabState extends State<_TextTab> {
  final _plainCtrl = TextEditingController(text: 'hello world!!!!!');
  String _status = '';
  bool _busy = false;
  bool _decryptMode = false;

  CipherView _view = CipherView.hex;
  PlainFormat _plainFormat = PlainFormat.text;

  Uint8List? _cachedLive;
  String _lastText = '';

  @override
  void initState() {
    super.initState();
    _plainCtrl.addListener(_onChanged);
    _cachedLive = _liveCipherBytes();
  }

  @override
  void didUpdateWidget(covariant _TextTab oldWidget) {
    super.didUpdateWidget(oldWidget);
    // If the parent changed cipher/mode/key/iv, recompute the live preview.
    if (oldWidget.cipher != widget.cipher ||
        oldWidget.mode   != widget.mode   ||
        oldWidget.keyText != widget.keyText ||
        oldWidget.ivText  != widget.ivText) {
      _cachedLive = _liveCipherBytes();
    }
  }

  @override
  void dispose() {
    _plainCtrl.removeListener(_onChanged);
    _plainCtrl.dispose();
    super.dispose();
  }

  void _onChanged() {
    if (_plainCtrl.text == _lastText) return;
    _lastText = _plainCtrl.text;
    setState(() {
      _cachedLive = _liveCipherBytes();
    });
  }

  String _bytesToHex(Uint8List b) =>
      b.map((x) => x.toRadixString(16).padLeft(2, '0')).join();

  Uint8List _pad(Uint8List data, int blockSize) {
    final padLen = blockSize - (data.length % blockSize);
    final padded = Uint8List(data.length + padLen);
    padded.setAll(0, data);
    padded.fillRange(data.length, padded.length, padLen);
    return padded;
  }

  void _setDecryptMode(bool value) {
    setState(() {
      _decryptMode = value;
      _plainCtrl.clear();
      _status = '';
    });
  }

  Uint8List? _parsePlain() {
    if (_plainFormat == PlainFormat.text) {
      return Uint8List.fromList(utf8.encode(_plainCtrl.text));
    }
    return widget.hexToBytes(_plainCtrl.text);
  }

  Uint8List? _applyCipher(Uint8List rawInput, int blockSize) {
    final key = widget.requireKey();
    final iv  = widget.mode == 'ecb' ? null : widget.requireIv();
    final plain = _pad(rawInput, blockSize);

    return _decryptMode
        ? SifarCipherMode.decrypt(
            cipher: widget.cipher, mode: widget.mode,
            input: plain, key: key, iv: iv,
          )
        : SifarCipherMode.encrypt(
            cipher: widget.cipher, mode: widget.mode,
            input: plain, key: key, iv: iv,
          );
  }

  Uint8List? _liveCipherBytes() {
    if (_plainCtrl.text.isEmpty) return null;
    if (_plainCtrl.text.length > 256) return null;

    try {
      final blockSize = blockSizeOf(widget.cipher);
      if (blockSize <= 0) return null;

      final raw = _parsePlain();
      if (raw == null) throw ArgumentError('Invalid hex input');

      final result = _applyCipher(raw, blockSize)!;

      if (_decryptMode && result.length >= raw.length) {
        return result.sublist(0, raw.length);
      }
      return result;
    } catch (_) {
      return null;
    }
  }

  String _renderCipher(Uint8List? bytes) {
    if (bytes == null) {
      return _decryptMode
          ? '(type ciphertext to see live plaintext)'
          : '(type plaintext to see live ciphertext)';
    }
    if (_decryptMode) {
      return utf8.decode(bytes, allowMalformed: true);
    }
    if (_view == CipherView.hex) return _bytesToHex(bytes);
    return String.fromCharCodes(bytes);
  }

  void _setPlainFormat(PlainFormat value) {
    setState(() {
      _plainFormat = value;
      _plainCtrl.clear();
      _status = '';
    });
  }

  void _run() {
    setState(() {
      _busy = true;
      _status = '';
    });
    try {
      final blockSize = blockSizeOf(widget.cipher);
      if (blockSize <= 0) throw ArgumentError('Unknown cipher: ${widget.cipher}');

      final raw = _parsePlain();
      if (raw == null) throw ArgumentError('Invalid hex input');

      final result = _applyCipher(raw, blockSize)!;

      if (_decryptMode) {
        final stripped = result.length >= raw.length
            ? result.sublist(0, raw.length)
            : result;
        setState(() {
          _busy = false;
          _status = 'PLAINTEXT:\n'
              '${utf8.decode(stripped, allowMalformed: true)}';
        });
      } else {
        setState(() {
          _busy = false;
          _status = 'CIPHER (${widget.cipher.toUpperCase()} / ${widget.mode.toUpperCase()}):\n'
              '${_bytesToHex(result)}';
        });
      }
    } catch (e) {
      setState(() {
        _busy = false;
        _status = 'ERROR: $e';
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      padding: EdgeInsets.fromLTRB(
        16, 8, 16, 16 + MediaQuery.of(context).viewInsets.bottom,
      ),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          SegmentedButton<bool>(
            segments: const [
              ButtonSegment(value: false, label: Text('Encrypt')),
              ButtonSegment(value: true,  label: Text('Decrypt')),
            ],
            selected: {_decryptMode},
            onSelectionChanged: (s) => _setDecryptMode(s.first),
          ),
          Row(
            children: [
              const Text('Plaintext format:', style: TextStyle(fontSize: 12)),
              const SizedBox(width: 8),
              SegmentedButton<PlainFormat>(
                segments: const [
                  ButtonSegment(value: PlainFormat.text, label: Text('Text')),
                  ButtonSegment(value: PlainFormat.hex, label: Text('Hex')),
                ],
                selected: {_plainFormat},
                onSelectionChanged: (s) => _setPlainFormat(s.first),
                style: const ButtonStyle(visualDensity: VisualDensity.compact),
              ),
            ],
          ),
          const SizedBox(height: 6),
          TextField(
            controller: _plainCtrl,
            maxLines: 3,
            decoration: InputDecoration(
              labelText: 'Plaintext',
              border: const OutlineInputBorder(),
              suffixIcon: _plainCtrl.text.isEmpty
                  ? null
                  : IconButton(
                      icon: const Icon(Icons.clear, size: 18),
                      tooltip: 'Clear',
                      onPressed: () => setState(() {
                        _plainCtrl.clear();
                        _status = '';
                      }),
                    ),
            ),
          ),
          const SizedBox(height: 8),
          Row(
            children: [
              const Text('Ciphertext view:', style: TextStyle(fontSize: 12)),
              const SizedBox(width: 8),
              SegmentedButton<CipherView>(
                segments: const [
                  ButtonSegment(value: CipherView.hex, label: Text('Hex')),
                  ButtonSegment(value: CipherView.latin1, label: Text('Latin-1')),
                ],
                selected: {_view},
                onSelectionChanged: (s) => setState(() => _view = s.first),
                style: const ButtonStyle(visualDensity: VisualDensity.compact),
              ),
            ],
          ),
          const SizedBox(height: 6),
          Stack(
            children: [
              Container(
                width: double.infinity,
                padding: const EdgeInsets.all(8),
                decoration: BoxDecoration(
                  color: Theme.of(context).colorScheme.surfaceContainerHighest,
                  borderRadius: BorderRadius.circular(6),
                ),
                child: SelectableText(
                  _renderCipher(_cachedLive),
                  style: const TextStyle(fontFamily: 'monospace', fontSize: 11),
                ),
              ),
              if (_cachedLive != null)
                Positioned(
                  top: 0,
                  right: 0,
                  child: IconButton(
                    icon: const Icon(Icons.copy, size: 16),
                    tooltip: 'Copy ciphertext',
                    onPressed: () {
                      final text = _renderCipher(_cachedLive);
                      Clipboard.setData(ClipboardData(text: text));
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(content: Text('Ciphertext copied')),
                      );
                    },
                  ),
                ),
            ],
          ),
          const SizedBox(height: 12),
          SizedBox(
            height: 180,
            child: Stack(
              children: [
                Container(
                  padding: const EdgeInsets.all(12),
                  decoration: BoxDecoration(
                    color: Theme.of(context).colorScheme.surfaceContainerHighest,
                    borderRadius: BorderRadius.circular(8),
                  ),
                  child: SingleChildScrollView(
                    child: SelectableText(
                      _status.isEmpty ? '(result here)' : _status,
                      style: const TextStyle(fontFamily: 'monospace', fontSize: 13),
                    ),
                  ),
                ),
                if (_status.isNotEmpty)
                  Positioned(
                    top: 4,
                    right: 4,
                    child: IconButton(
                      icon: const Icon(Icons.copy, size: 18),
                      tooltip: 'Copy',
                      onPressed: () {
                        Clipboard.setData(ClipboardData(text: _status));
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(content: Text('Copied')),
                        );
                      },
                    ),
                  ),
              ],
            ),
          ),
          const SizedBox(height: 12),
          FilledButton(
            onPressed: _busy ? null : _run,
            style: FilledButton.styleFrom(
              padding: const EdgeInsets.symmetric(vertical: 16),
            ),
            child: Text(_decryptMode ? 'Decrypt Text' : 'Encrypt Text'),
          ),
        ],
      ),
    );
  }
}

// ============================================================
// IMAGE TAB
// ============================================================
class _ImageTab extends StatefulWidget {
  final String cipher;
  final String mode;
  final Uint8List Function() requireKey;

  const _ImageTab({
    required this.cipher,
    required this.mode,
    required this.requireKey,
  });

  @override
  State<_ImageTab> createState() => _ImageTabState();
}

class _ImageTabState extends State<_ImageTab> {
  bool _busy = false;
  String _status = '';
  String? _encPath;
  String? _decPath;

  Future<({String input, String enc, String dec})?> _pickAndStage() async {
    final picked = await ImagePicker().pickImage(source: ImageSource.gallery);
    if (picked == null) return null;

    final dir = await getApplicationDocumentsDirectory();
    final stamp = DateTime.now().millisecondsSinceEpoch;
    final input = '${dir.path}/input_$stamp.png';
    final enc   = '${dir.path}/encrypted_$stamp.png';
    final dec   = '${dir.path}/decrypted_$stamp.png';

    await File(picked.path).copy(input);
    return (input: input, enc: enc, dec: dec);
  }

  void _beginOp() => setState(() {
        _busy = true;
        _status = '';
        _encPath = null;
        _decPath = null;
      });

  void _error(Object e) => setState(() {
        _busy = false;
        _status = 'ERROR: $e';
      });

  Future<void> _encrypt() async {
    _beginOp();
    try {
      final p = await _pickAndStage();
      if (p == null) return setState(() => _busy = false);

      SifarImage.encryptImage(
        cipher: widget.cipher,
        inputPath: p.input,
        outputPath: p.enc,
        key: widget.requireKey(),
      );
      setState(() {
        _busy = false;
        _encPath = p.enc;
      });
    } catch (e) {
      _error(e);
    }
  }

  Future<void> _decrypt() async {
    _beginOp();
    try {
      final p = await _pickAndStage();
      if (p == null) return setState(() => _busy = false);

      SifarImage.decryptImage(
        cipher: widget.cipher,
        inputPath: p.input,
        outputPath: p.dec,
        key: widget.requireKey(),
      );
      setState(() {
        _busy = false;
        _decPath = p.dec;
      });
    } catch (e) {
      _error(e);
    }
  }

  Future<void> _roundTrip() async {
    _beginOp();
    try {
      final p = await _pickAndStage();
      if (p == null) return setState(() => _busy = false);

      final key = widget.requireKey();
      SifarImage.encryptImage(
        cipher: widget.cipher, inputPath: p.input, outputPath: p.enc, key: key);
      SifarImage.decryptImage(
        cipher: widget.cipher, inputPath: p.enc, outputPath: p.dec, key: key);

      final orig = await File(p.input).readAsBytes();
      final dec  = await File(p.dec).readAsBytes();
      final match = orig.length == dec.length &&
          List.generate(orig.length, (i) => orig[i] == dec[i])
              .every((x) => x);

      setState(() {
        _busy = false;
        _encPath = p.enc;
        _decPath = p.dec;
        _status = match
            ? '✅ Round-trip OK (${orig.length} bytes)'
            : '❌ Mismatch: orig=${orig.length} dec=${dec.length}';
      });
    } catch (e) {
      _error(e);
    }
  }

  Future<void> _saveToGallery(String path, String label) async {
    try {
      // need a File reference — save from the sandbox path
      await Gal.putImage(path, album: 'Sifar');
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('$label saved to gallery')),
        );
      }
    } on GalException catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Save failed: ${e.type.message}')),
        );
      }
    }
  }

  Widget _panel() {
    if (_encPath == null && _decPath == null) {
      return SelectableText(
        _status.isEmpty ? '(result here)' : _status,
        style: const TextStyle(fontFamily: 'monospace', fontSize: 13),
      );
    }
    return Column(
      children: [
        if (_encPath != null) ...[
          const Text('ENCRYPTED', style: TextStyle(fontWeight: FontWeight.bold)),
          const SizedBox(height: 8),
          Image.file(File(_encPath!)),
          TextButton.icon(
            icon: const Icon(Icons.save_alt, size: 16),
            label: const Text('Save to gallery'),
            onPressed: () => _saveToGallery(_encPath!, 'Encrypted image'),
          ),
        ],
        if (_decPath != null) ...[
          const SizedBox(height: 16),
          const Text('DECRYPTED', style: TextStyle(fontWeight: FontWeight.bold)),
          const SizedBox(height: 8),
          Image.file(File(_decPath!)),
          TextButton.icon(
            icon: const Icon(Icons.save_alt, size: 16),
            label: const Text('Save to gallery'),
            onPressed: () => _saveToGallery(_encPath!, 'Encrypted image'),
          ),

        ],
        if (_status.isNotEmpty) ...[
          const SizedBox(height: 16),
          Text(_status,
              textAlign: TextAlign.center,
              style: const TextStyle(fontFamily: 'monospace', fontSize: 13)),
        ],
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.fromLTRB(16, 8, 16, 16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          if (widget.mode != 'ecb')
            Padding(
              padding: const EdgeInsets.only(bottom: 8),
              child: Text(
                'Note: image encryption uses ECB regardless of mode.',
                style: TextStyle(
                  color: Theme.of(context).colorScheme.error,
                  fontSize: 12,
                ),
              ),
            ),
          Expanded(
            child: Container(
              padding: const EdgeInsets.all(12),
              decoration: BoxDecoration(
                color: Theme.of(context).colorScheme.surfaceContainerHighest,
                borderRadius: BorderRadius.circular(8),
              ),
              child: SingleChildScrollView(child: _panel()),
            ),
          ),
          const SizedBox(height: 12),
          Row(
            children: [
              Expanded(
                child: FilledButton(
                  onPressed: _busy ? null : _encrypt,
                  style: FilledButton.styleFrom(
                    padding: const EdgeInsets.symmetric(vertical: 16),
                  ),
                  child: const Text('Encrypt'),
                ),
              ),
              const SizedBox(width: 8),
              Expanded(
                child: FilledButton.tonal(
                  onPressed: _busy ? null : _decrypt,
                  style: FilledButton.styleFrom(
                    padding: const EdgeInsets.symmetric(vertical: 16),
                  ),
                  child: const Text('Decrypt'),
                ),
              ),
            ],
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
    );
  }
}

// ============================================================
// HASH TAB
// ============================================================
class _HashTab extends StatefulWidget {
  const _HashTab();

  @override
  State<_HashTab> createState() => _HashTabState();
}

enum HashFormat { text, hex }
enum HashView { hex, base64 }

class _HashTabState extends State<_HashTab> {
  final _inputCtrl = TextEditingController();
  String _hash = 'sha256';
  HashFormat _format = HashFormat.text;
  HashView _view = HashView.hex;

  Uint8List? _cachedHash;

  @override
  void initState() {
    super.initState();
    _inputCtrl.addListener(_onChanged);
  }

  @override
  void dispose() {
    _inputCtrl.removeListener(_onChanged);
    _inputCtrl.dispose();
    super.dispose();
  }

  void _onChanged() {
    setState(() {
      _cachedHash = _liveHash();
    });
  }

  Uint8List? _parseInput() {
    if (_inputCtrl.text.isEmpty) return null;
    if (_format == HashFormat.text) {
      return Uint8List.fromList(utf8.encode(_inputCtrl.text));
    }
    final hex = _inputCtrl.text.replaceAll(RegExp(r'\s'), '');
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

  Uint8List? _liveHash() {
    final input = _parseInput();
    if (input == null) return null;
    try {
      return SifarHash.hash(_hash, input);
    } catch (_) {
      return null;
    }
  }

  String _renderHash(Uint8List? bytes) {
    if (bytes == null) return '(type input to see hash)';
    if (_view == HashView.hex) {
      return bytes.map((x) => x.toRadixString(16).padLeft(2, '0')).join();
    }
    return base64.encode(bytes);
  }

  @override
  Widget build(BuildContext context) {
    final live = _cachedHash;

    return SingleChildScrollView(
      padding: EdgeInsets.fromLTRB(
        16, 8, 16, 16 + MediaQuery.of(context).viewInsets.bottom,
      ),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Row(
            children: [
              const Text('Input format: ', style: TextStyle(fontSize: 12)),
              const SizedBox(width: 8),
              SegmentedButton<HashFormat>(
                segments: const [
                  ButtonSegment(value: HashFormat.text, label: Text('Text')),
                  ButtonSegment(value: HashFormat.hex,  label: Text('Hex')),
                ],
                selected: {_format},
                onSelectionChanged: (s) => setState(() {
                  _format = s.first;
                  _inputCtrl.clear();
                }),
                style: const ButtonStyle(visualDensity: VisualDensity.compact),
              ),
            ],
          ),
          const SizedBox(height: 8),
          Row(
            children: [
              const Text('Hash: '),
              const SizedBox(width: 8),
              Expanded(
                child: DropdownButton<String>(
                  isExpanded: true,
                  value: _hash,
                  onChanged: (v) => setState(() => _hash = v!),
                  items: SifarHash.names
                      .map((h) => DropdownMenuItem(
                            value: h,
                            child: Text(h.toUpperCase()),
                          ))
                      .toList(),
                ),
              ),
            ],
          ),
          const SizedBox(height: 12),
          TextField(
            controller: _inputCtrl,
            maxLines: 3,
            decoration: InputDecoration(
              labelText: 'Input',
              border: const OutlineInputBorder(),
              suffixIcon: _inputCtrl.text.isEmpty
                  ? null
                  : IconButton(
                      icon: const Icon(Icons.clear, size: 18),
                      tooltip: 'Clear',
                      onPressed: () => setState(() {
                        _inputCtrl.clear();
                      }),
                    ),
            ),
          ),
          const SizedBox(height: 8),
          Row(
            children: [
              const Text('Output view: ', style: TextStyle(fontSize: 12)),
              const SizedBox(width: 8),
              SegmentedButton<HashView>(
                segments: const [
                  ButtonSegment(value: HashView.hex,    label: Text('Hex')),
                  ButtonSegment(value: HashView.base64, label: Text('Base64')),
                ],
                selected: {_view},
                onSelectionChanged: (s) => setState(() => _view = s.first),
                style: const ButtonStyle(visualDensity: VisualDensity.compact),
              ),
            ],
          ),
          const SizedBox(height: 6),
          Stack(
            children: [
              Container(
                width: double.infinity,
                padding: const EdgeInsets.all(12),
                decoration: BoxDecoration(
                  color: Theme.of(context).colorScheme.surfaceContainerHighest,
                  borderRadius: BorderRadius.circular(8),
                ),
                child: SelectableText(
                  _renderHash(live),
                  style: const TextStyle(fontFamily: 'monospace', fontSize: 13),
                ),
              ),
              if (live != null)
                Positioned(
                  top: 4,
                  right: 4,
                  child: IconButton(
                    icon: const Icon(Icons.copy, size: 18),
                    tooltip: 'Copy hash',
                    onPressed: () {
                      Clipboard.setData(ClipboardData(text: _renderHash(live)));
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(content: Text('Hash copied')),
                      );
                    },
                  ),
                ),
            ],
          ),
        ],
      ),
    );
  }
}