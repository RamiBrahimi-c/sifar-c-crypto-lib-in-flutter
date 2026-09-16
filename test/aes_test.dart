import 'dart:convert';
import 'dart:typed_data';
import 'package:my_native_wrapper/sifar.dart';
import 'package:test/test.dart';

void main() {
  group('SifarAes round-trip', () {
    test('AES-128', () => _roundTrip(16));
    test('AES-192', () => _roundTrip(24));
    test('AES-256', () => _roundTrip(32));

    test('different keys produce different ciphertext', () {
      final plain = _hex('00112233445566778899aabbccddeeff');
      final k1 = Uint8List.fromList(List.filled(16, 0x01));
      final k2 = Uint8List.fromList(List.filled(16, 0x02));

      final a1 = SifarAes(k1);
      final a2 = SifarAes(k2);
      final c1 = a1.encrypt(plain);
      final c2 = a2.encrypt(plain);
      a1.dispose();
      a2.dispose();

      expect(_hexOut(c1), isNot(equals(_hexOut(c2))));
    });
  });

  group('FIPS-197 known-answer tests', () {
    test('AES-128 (Appendix C.1)', () {
      _kat(
        key:      '000102030405060708090a0b0c0d0e0f',
        plain:    '00112233445566778899aabbccddeeff',
        expected: '69c4e0d86a7b0430d8cdb78070b4c55a',
      );
    });

    test('AES-192 (Appendix C.2)', () {
      _kat(
        key:      '000102030405060708090a0b0c0d0e0f1011121314151617',
        plain:    '00112233445566778899aabbccddeeff',
        expected: 'dda97ca4864cdfe06eaf70a0ec0d7191',
      );
    });

    test('AES-256 (Appendix C.3)', () {
      _kat(
        key:      '000102030405060708090a0b0c0d0e0f'
                  '101112131415161718191a1b1c1d1e1f',
        plain:    '00112233445566778899aabbccddeeff',
        expected: '8ea2b7ca516745bfeafc49904b496089',
      );
    });
  });
}

void _roundTrip(int keyLen) {
  final key = Uint8List.fromList(List.filled(keyLen, 0x2b));
  final aes = SifarAes(key);
  final plain = Uint8List.fromList(utf8.encode('1234567890abcdef'));
  final enc = aes.encrypt(plain);
  final dec = aes.decrypt(enc);
  aes.dispose();

  expect(enc, isNot(equals(plain)),
      reason: 'ciphertext should differ from plaintext');
  expect(dec, equals(plain),
      reason: 'decrypt(encrypt(x)) must equal x');
}

void _kat({
  required String key,
  required String plain,
  required String expected,
}) {
  final aes = SifarAes(_hex(key));
  final enc = aes.encrypt(_hex(plain));
  aes.dispose();

  expect(_hexOut(enc), equals(expected));
}

Uint8List _hex(String s) => Uint8List.fromList(
    List.generate(s.length ~/ 2,
        (i) => int.parse(s.substring(i * 2, i * 2 + 2), radix: 16)));

String _hexOut(Uint8List b) =>
    b.map((x) => x.toRadixString(16).padLeft(2, '0')).join();