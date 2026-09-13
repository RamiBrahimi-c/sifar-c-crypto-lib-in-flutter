import 'dart:convert';
import 'dart:typed_data';
import 'package:my_native_wrapper/sifar.dart';
import 'package:test/test.dart';

void main() {
  test('AES encrypt/decrypt round-trip', () {
    final key = Uint8List.fromList(List.filled(16, 0x2b));
    final aes = SifarAes(key);
    final plain = Uint8List.fromList(utf8.encode('1234567890abcdef'));
    final enc = aes.encrypt(plain);
    final dec = aes.decrypt(enc);
    expect(dec, equals(plain));
    aes.dispose();
  });
}