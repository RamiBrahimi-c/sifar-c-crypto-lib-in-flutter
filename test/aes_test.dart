import 'dart:convert';
import 'dart:typed_data';
import 'package:my_native_wrapper/sifar.dart';
import 'package:test/test.dart';

void main() {
  group('SifarAes', () {
    test('round-trip on 16 bytes', () {
      final key = Uint8List.fromList(List.filled(16, 0x2b));
      final aes = SifarAes(key);
      final plain = Uint8List.fromList(utf8.encode('1234567890abcdef'));
      final enc = aes.encrypt(plain);
      final dec = aes.decrypt(enc);
      aes.dispose();

      expect(enc, isNot(equals(plain)),
          reason: 'ciphertext should differ from plaintext');
      expect(dec, equals(plain),
          reason: 'decrypt(encrypt(x)) must equal x');
    });

    test('FIPS-197 known-answer test (AES-128)', () {
      // From FIPS-197 Appendix C.1
      final key      = _hex('000102030405060708090a0b0c0d0e0f');
      final plain    = _hex('00112233445566778899aabbccddeeff');
      final expected = _hex('69c4e0d86a7b0430d8cdb78070b4c55a');

      final aes = SifarAes(key);
      final enc = aes.encrypt(plain);
      aes.dispose();

      expect(_hexOut(enc), equals(_hexOut(expected)),
          reason: 'AES-128 must match FIPS-197 test vector');
    });
  });
}

Uint8List _hex(String s) => Uint8List.fromList(
    List.generate(s.length ~/ 2,
        (i) => int.parse(s.substring(i * 2, i * 2 + 2), radix: 16)));

String _hexOut(Uint8List b) =>
    b.map((x) => x.toRadixString(16).padLeft(2, '0')).join();