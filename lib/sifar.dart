import 'dart:ffi';
import 'dart:typed_data';
import 'package:ffi/ffi.dart';
import 'my_native_wrapper_bindings_generated.dart' as sifar;
import 'dart:ffi' as ffi;
import 'my_native_wrapper_bindings_generated.dart' as bindings;

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
}