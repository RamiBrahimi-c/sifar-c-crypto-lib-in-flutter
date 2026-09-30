import 'dart:convert';
import 'dart:typed_data';
import 'package:my_native_wrapper/sifar.dart';
import 'package:test/test.dart';

void main() {
  group('Hash known-answer tests', () {
    // Empty input
    test('MD5("")', () {
      final d = SifarHash.hash('md5', Uint8List(0));
      expect(_hex(d), 'd41d8cd98f00b204e9800998ecf8427e');
    });

    test('SHA-256("")', () {
      final d = SifarHash.hash('sha256', Uint8List(0));
      expect(_hex(d), 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855');
    });

    test('SHA-512("")', () {
      final d = SifarHash.hash('sha512', Uint8List(0));
      expect(_hex(d), 'cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce'
                      '47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e');
    });

    // "abc"
    test('MD4("abc")', () {
      final d = SifarHash.hash('md4', Uint8List.fromList(utf8.encode('abc')));
      expect(_hex(d), 'a448017aaf21d8525fc10ae87aa6729d');
    });

    test('MD5("abc")', () {
      final d = SifarHash.hash('md5', Uint8List.fromList(utf8.encode('abc')));
      expect(_hex(d), '900150983cd24fb0d6963f7d28e17f72');
    });

    test('SHA-256("abc")', () {
      final d = SifarHash.hash('sha256', Uint8List.fromList(utf8.encode('abc')));
      expect(_hex(d), 'ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad');
    });

    test('SHA-512("abc")', () {
      final d = SifarHash.hash('sha512', Uint8List.fromList(utf8.encode('abc')));
      expect(_hex(d), 'ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a'
                      '2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f');
    });
  });

    test('debug sha256', () {
    for (final s in ['', 'a', 'abc', 'abcd', 'hello world']) {
      final d = SifarHash.hash('sha256', Uint8List.fromList(utf8.encode(s)));
      // ignore: avoid_print
      print('sha256("$s") = ${_hex(d)}');
    }
  });
}

String _hex(Uint8List b) =>
    b.map((x) => x.toRadixString(16).padLeft(2, '0')).join();