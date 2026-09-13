import 'package:native_toolchain_c/native_toolchain_c.dart';
import 'package:logging/logging.dart';
import 'package:hooks/hooks.dart';

void main(List<String> args) async {
  await build(args, (input, output) async {
    final packageName = input.packageName;

    final cbuilder = CBuilder.library(
      name: packageName,
      assetName: '${packageName}_bindings_generated.dart',
      sources: [
        'src/sifar_ffi.c',
        'src/block_cipher_modes_operation.c',
        'src/common/constants.c',
        'src/common/custom_string.c',
        'src/common/galois_field_op.c',
        'src/common/keyexpan.c',
        'src/common/rijnbox.c',
        'src/common/utils.c',
        'src/ciphers/classical/affine.c',
        'src/ciphers/classical/caesar.c',
        'src/ciphers/classical/hill.c',
        'src/ciphers/classical/playfair.c',
        'src/ciphers/classical/substitution.c',
        'src/ciphers/classical/vigenere.c',
        'src/ciphers/symmetric/aes.c',
        'src/ciphers/symmetric/blowfish.c',
        'src/ciphers/symmetric/des.c',
        'src/ciphers/symmetric/rc4.c',
        'src/ciphers/symmetric/redpike.c',
        'src/ciphers/symmetric/reseau_fistel.c',
        'src/ciphers/symmetric/tea.c',
        'src/ciphers/symmetric/xtea.c',
      ],
      includes: [
        'src',
        'src/common',
        'src/ciphers',
        'src/ciphers/symmetric',
        'src/ciphers/classical',
      ],
    );

    await cbuilder.run(
      input: input,
      output: output,
      logger: Logger('')
        ..level = .ALL
        ..onRecord.listen((record) => print(record.message)),
    );
  });
}