import 'dart:convert';
import 'dart:typed_data';
import 'package:my_native_wrapper/sifar.dart';
import 'package:test/test.dart';

void main() {
  final key = Uint8List.fromList(List.generate(16, (i) => i));
  final iv  = Uint8List.fromList(List.generate(16, (i) => 0x10 + i));
  final input = Uint8List.fromList(utf8.encode('hello world!!!!!'));

  for (final mode in ['ecb', 'cbc', 'cfb', 'ofb', 'ctr']) {
    test('AES $mode round-trip', () {
      final enc = SifarCipherMode.encrypt(
        cipher: 'aes', mode: mode, input: input, key: key,
        iv: mode == 'ecb' ? null : iv,
      );
      final dec = SifarCipherMode.decrypt(
        cipher: 'aes', mode: mode, input: enc, key: key,
        iv: mode == 'ecb' ? null : iv,
      );
      expect(dec, equals(input));
    });
  }
}