import 'dart:convert';
import 'dart:ffi';
import 'dart:typed_data';
import 'package:ffi/ffi.dart';
import 'my_native_wrapper_bindings_generated.dart' as sifar;
import 'dart:ffi' as ffi;
import 'my_native_wrapper_bindings_generated.dart' as bindings;


/// Returns the block size in bytes for the given cipher name.
/// Returns 0 if the cipher is unknown.
int blockSizeOf(String cipher) {
  final ptr = cipher.toNativeUtf8();
  try {
    return sifar.get_block_size(ptr.cast<ffi.Char>());
  } finally {
    malloc.free(ptr);
  }
}

/// Returns expected key size in bytes for the given cipher.
/// Returns -1 for variable-length ciphers (blowfish, rc4).
/// Returns 0 for unknown ciphers.
int keySizeOf(String cipher) {
  final ptr = cipher.toNativeUtf8();
  try {
    return sifar.get_key_size(ptr.cast<ffi.Char>());
  } finally {
    malloc.free(ptr);
  }
}


int digestSizeOf(String hash) {
  final ptr = hash.toNativeUtf8();
  try {
    return sifar.get_digest_size(ptr.cast<ffi.Char>());
  } finally {
    malloc.free(ptr);
  }
}

class SifarHash {
  static const List<String> names = ['md4', 'md5', 'sha256', 'sha512'];

  /// Returns the raw digest bytes of [input] using [name].
  static Uint8List hash(String name, Uint8List input) {
    final size = digestSizeOf(name);
    if (size <= 0) throw ArgumentError('Unknown hash: $name');

    final inPtr  = calloc<Uint8>(input.isEmpty ? 1 : input.length);
    final outPtr = calloc<Uint8>(size);

    try {
      if (input.isNotEmpty) {
        inPtr.asTypedList(input.length).setAll(0, input);
      }

      switch (name) {
        case 'md4':
          sifar.md4_hash(inPtr.cast(), input.length, outPtr.cast());
          break;
        case 'md5':
          sifar.md5_hash(inPtr.cast(), input.length, outPtr.cast());
          break;
        case 'sha256':
          sifar.sha256_hash(inPtr.cast(), input.length, outPtr.cast());
          break;
        case 'sha512':
          sifar.sha512_hash(inPtr.cast(), input.length, outPtr.cast());
          break;
      }

      return Uint8List.fromList(outPtr.asTypedList(size));
    } finally {
      calloc.free(inPtr);
      calloc.free(outPtr);
    }
  }
}

/// Convert bytes to base64 string.
String bytesToBase64(Uint8List bytes) => base64.encode(bytes);


class SifarCipher {
  final String name;
  final Pointer<Void> _handle;

  SifarCipher._(this.name, this._handle);

  /// Build a cipher handle from [key] bytes.
  /// [name] must be one of: aes, des, rc4, redpike, tea, xtea.
  factory SifarCipher(String name, Uint8List key) {
    final keyPtr = calloc<Uint8>(key.length);
    keyPtr.asTypedList(key.length).setAll(0, key);

    final Pointer<Void> handle;
    switch (name) {
      case 'aes':       handle = sifar.aes_new_key(keyPtr.cast(), key.length); break;
      case 'des':       handle = sifar.des_new_key(keyPtr.cast(), key.length); break;
      case '3des':      handle = sifar.tdes_new_key(keyPtr.cast(), key.length); break;
      case 'blowfish':  handle = sifar.blowfish_new_key(keyPtr.cast(), key.length); break;
      case 'rc4':       handle = sifar.rc4_new_key(keyPtr.cast(), key.length); break;
      case 'redpike':   handle = sifar.redpike_new_key(keyPtr.cast(), key.length); break;
      case 'tea':       handle = sifar.tea_new_key(keyPtr.cast(), key.length); break;
      case 'xtea':      handle = sifar.xtea_new_key(keyPtr.cast(), key.length); break;
      default:
        calloc.free(keyPtr);
        throw ArgumentError('Unknown cipher: $name');
    }

    calloc.free(keyPtr);
    if (handle == nullptr) throw StateError('$name: key setup failed');
    return SifarCipher._(name, handle);
  }

  Uint8List encrypt(Uint8List input) => _run(input, encrypt: true);
  Uint8List decrypt(Uint8List input) => _run(input, encrypt: false);

  Uint8List _run(Uint8List input, {required bool encrypt}) {
    final inPtr  = calloc<Uint8>(input.length);
    final outPtr = calloc<Uint8>(input.length);
    try {
      inPtr.asTypedList(input.length).setAll(0, input);

      final rc = _dispatch(inPtr, outPtr, input.length, encrypt);
      if (rc != 0) throw StateError('$name ${encrypt ? "encrypt" : "decrypt"} failed: $rc');

      return Uint8List.fromList(outPtr.asTypedList(input.length));
    } finally {
      calloc.free(inPtr);
      calloc.free(outPtr);
    }
  }

  int _dispatch(Pointer<Uint8> inPtr, Pointer<Uint8> outPtr, int len, bool enc) {
    switch (name) {
      case 'aes':
        return enc
            ? sifar.aes_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.aes_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case 'des':
        return enc
            ? sifar.des_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.des_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case '3des':
        return enc
            ? sifar.tdes_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.tdes_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case 'blowfish':
        return enc
            ? sifar.blowfish_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.blowfish_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case 'rc4':
        return enc
            ? sifar.rc4_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.rc4_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case 'redpike':
        return enc
            ? sifar.redpike_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.redpike_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case 'tea':
        return enc
            ? sifar.tea_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.tea_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      case 'xtea':
        return enc
            ? sifar.xtea_encrypt(inPtr.cast(), outPtr.cast(), len, _handle)
            : sifar.xtea_decrypt(inPtr.cast(), outPtr.cast(), len, _handle);
      default:
        return -1;
    }
  }

  void dispose() {
    switch (name) {
      case 'aes':           sifar.aes_destroy_key(_handle); break;
      case 'des':           sifar.des_destroy_key(_handle); break;
      case '3des':          sifar.tdes_destroy_key(_handle); break;
      case 'blowfish':      sifar.blowfish_destroy_key(_handle); break;
      case 'rc4':           sifar.rc4_destroy_key(_handle); break;
      case 'redpike':       sifar.redpike_destroy_key(_handle); break;
      case 'tea':           sifar.tea_destroy_key(_handle); break;
      case 'xtea':          sifar.xtea_destroy_key(_handle); break;
    }
  }
}


class SifarAes {
  final Pointer<Void> _handle;

  SifarAes._(this._handle);

  /// Create a new AES key handle from raw key bytes.
  /// [keyBytes] should be 16, 24, or 32 bytes (AES-128/192/256).
  factory SifarAes(Uint8List keyBytes) {
    final keyPtr = calloc<Uint8>(keyBytes.length);
    keyPtr.asTypedList(keyBytes.length).setAll(0, keyBytes);
    final handle = sifar.aes_new_key(keyPtr.cast(), keyBytes.length);
    calloc.free(keyPtr);
    if (handle == nullptr) {
      throw StateError('aes_new_key failed');
    }
    return SifarAes._(handle);
  }

  /// Encrypt [plaintext]. Length should be a multiple of the AES block size (16).
  Uint8List encrypt(Uint8List plaintext) {
    final inPtr  = calloc<Uint8>(plaintext.length);
    final outPtr = calloc<Uint8>(plaintext.length);
    try {
      inPtr.asTypedList(plaintext.length).setAll(0, plaintext);
      final rc = sifar.aes_encrypt(inPtr.cast(), outPtr.cast(), plaintext.length, _handle);
      if (rc != 0) throw StateError('aes_encrypt failed: $rc');
      return Uint8List.fromList(outPtr.asTypedList(plaintext.length));
    } finally {
      calloc.free(inPtr);
      calloc.free(outPtr);
    }
  }

  /// Decrypt [ciphertext].
  Uint8List decrypt(Uint8List ciphertext) {
    final inPtr  = calloc<Uint8>(ciphertext.length);
    final outPtr = calloc<Uint8>(ciphertext.length);
    try {
      inPtr.asTypedList(ciphertext.length).setAll(0, ciphertext);
      final rc = sifar.aes_decrypt(inPtr.cast(), outPtr.cast(), ciphertext.length, _handle);
      if (rc != 0) throw StateError('aes_decrypt failed: $rc');
      return Uint8List.fromList(outPtr.asTypedList(ciphertext.length));
    } finally {
      calloc.free(inPtr);
      calloc.free(outPtr);
    }
  }

  /// Must be called when done to free the C-side key struct.
  void dispose() {
    sifar.aes_destroy_key(_handle);
  }


  
}


class SifarImage {
  static void encryptImage({
    required String cipher,
    required String inputPath,
    required String outputPath,
    required Uint8List key,
  }) {
    final namePtr = cipher.toNativeUtf8();      // Pointer<Utf8>
    final inPtr   = inputPath.toNativeUtf8();
    final outPtr  = outputPath.toNativeUtf8();
    final keyPtr  = calloc<Uint8>(key.length);
    keyPtr.asTypedList(key.length).setAll(0, key);

    try {
      final rc = sifar.encrypt_image_file(
        namePtr.cast<ffi.Char>(),               // Pointer<Char>
        inPtr.cast<bindings.uchar_t>(),         // Pointer<uchar_t>
        outPtr.cast<bindings.uchar_t>(),
        keyPtr.cast<bindings.uchar_t>(),
        key.length,
      );
      if (rc != 0) throw StateError('encrypt_image_file failed: $rc');
    } finally {
      malloc.free(namePtr);
      malloc.free(inPtr);
      malloc.free(outPtr);
      calloc.free(keyPtr);
    }
  }

  static void decryptImage({
    required String cipher,
    required String inputPath,
    required String outputPath,
    required Uint8List key,
  }) {
    final namePtr = cipher.toNativeUtf8();
    final inPtr   = inputPath.toNativeUtf8();
    final outPtr  = outputPath.toNativeUtf8();
    final keyPtr  = calloc<Uint8>(key.length);
    keyPtr.asTypedList(key.length).setAll(0, key);

    try {
      final rc = sifar.decrypt_image_file(
        namePtr.cast<ffi.Char>(),
        inPtr.cast<bindings.uchar_t>(),
        outPtr.cast<bindings.uchar_t>(),
        keyPtr.cast<bindings.uchar_t>(),
        key.length,
      );
      if (rc != 0) throw StateError('decrypt_image_file failed: $rc');
    } finally {
      malloc.free(namePtr);
      malloc.free(inPtr);
      malloc.free(outPtr);
      calloc.free(keyPtr);
    }
  }
}