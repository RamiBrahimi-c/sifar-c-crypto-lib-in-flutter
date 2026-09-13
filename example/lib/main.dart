import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:my_native_wrapper/sifar.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});
  @override
  Widget build(BuildContext context) => MaterialApp(home: const Home());
}

class Home extends StatefulWidget {
  const Home({super.key});
  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  String _result = '(not run)';

  void _run() {
    try {
      // 16-byte key = AES-128
      final key = Uint8List.fromList(List<int>.filled(16, 0x2b));
      final aes = SifarAes(key);

      final plain = Uint8List.fromList(utf8.encode('1234567890abcdef')); // 16 bytes
      final enc = aes.encrypt(plain);
      final dec = aes.decrypt(enc);

      setState(() {
        _result = 'plain: ${plain}\n'
                  'enc:   ${enc.map((b) => b.toRadixString(16).padLeft(2, '0')).join()}\n'
                  'dec:   ${utf8.decode(dec)}';
      });

      aes.dispose();
    } catch (e) {
      setState(() => _result = 'ERROR: $e');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Sifar AES')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            ElevatedButton(onPressed: _run, child: const Text('Run AES')),
            const SizedBox(height: 16),
            Text(_result),
          ],
        ),
      ),
    );
  }
}