import 'package:flutter/material.dart';

class AboutPage extends StatelessWidget {
  const AboutPage({super.key});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return Scaffold(
      appBar: AppBar(title: const Text('About Sifar')),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(20),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            // ---- Title ----
            Text(
              'Sifar',
              style: theme.textTheme.headlineMedium?.copyWith(
                fontWeight: FontWeight.bold,
              ),
            ),
            const SizedBox(height: 4),
            Text(
              'A demo app for a from-scratch crypto library ',
              style: theme.textTheme.titleMedium?.copyWith(
                color: theme.colorScheme.onSurfaceVariant,
              ),
            ),

            const SizedBox(height: 24),

            // ---- What it is ----
            _SectionTitle('What this is'),
            const _Paragraph(
              'An Android demo app that exercises the Sifar C cryptography '
              'library through Dart FFI. Every cipher, mode, and hash here is '
              'implemented from scratch in C — no OpenSSL, no libsodium, no '
              'third-party crypto.',
            ),
            const _Paragraph(
              'This app is a demonstration, not a production tool. The code '
              'is a learning exercise in cross-language integration and '
              'cryptographic implementation.'
              ,
            ),
            const _Paragraph(
              'I wanted to see the source lib sifar made in C (github.com/RamiBrahimi-c/cryptography-library) '
              'work on phone and work back to back with modern language , also it is fun to visualize encryption on images '
              'or watch how basic text turns into random garbage symbols!!!'
            ),

            const SizedBox(height: 16),

            // ---- Capabilities ----
            _SectionTitle('What it does'),
            _Bullet('Symmetric ciphers: AES, DES, 3DES, Blowfish, RC4, TEA, XTEA, Redpike'),
            _Bullet('Block modes: ECB, CBC, CFB, OFB, CTR'),
            _Bullet('Hashes: MD4, MD5, SHA-256, SHA-512'),
            _Bullet('Encrypt / decrypt text'),
            _Bullet('Encrypt / decrypt images (PNG via stb_image)'),
            _Bullet('Live preview of ciphertext and hashes'),

            const SizedBox(height: 16),

            // ---- Structure ----
            _SectionTitle('Architecture'),
            const _Paragraph(
              'Dart UI  →  FFI bindings (ffigen)  →  libmy_native_wrapper.so  '
              '→  Sifar C library',
            ),
            const _Paragraph(
              'The C library is cross-compiled for Android by the NDK at build '
              'time. Native calls from Dart go directly through dart:ffi — no '
              'platform channels, no JNI.',
            ),

            const SizedBox(height: 16),

            // ---- Warning ----
            _SectionTitle('⚠️ Not for real use'),
            Container(
              padding: const EdgeInsets.all(12),
              decoration: BoxDecoration(
                color: theme.colorScheme.errorContainer,
                borderRadius: BorderRadius.circular(8),
              ),
              child: Text(
                'This software has not been audited. Do not use it to protect '
                'anything that actually matters. Use a vetted library like '
                'libsodium or OpenSSL for real cryptographic work.',
                style: TextStyle(color: theme.colorScheme.onErrorContainer),
              ),
            ),

            const SizedBox(height: 24),

            // ---- Footer ----
            Center(
              child: Text(
                'Built as a fun/learning project',
                style: theme.textTheme.bodySmall?.copyWith(
                  color: theme.colorScheme.onSurfaceVariant,
                ),
              ),
            ),
            const SizedBox(height: 4),
            Center(
              child: SelectableText(
                'Source: github.com/RamiBrahimi-c/sifar-c-crypto-lib-in-flutter \n'
                'Main Sifar Lib in c: github.com/RamiBrahimi-c/cryptography-library \n'
                'by @main.c'
                ,
                style: theme.textTheme.bodySmall?.copyWith(
                  color: theme.colorScheme.onSurfaceVariant,
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

// ---- Small helpers ----

class _SectionTitle extends StatelessWidget {
  final String text;
  const _SectionTitle(this.text);

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 6),
      child: Text(
        text,
        style: Theme.of(context).textTheme.titleLarge?.copyWith(
              fontWeight: FontWeight.w600,
            ),
      ),
    );
  }
}

class _Paragraph extends StatelessWidget {
  final String text;
  const _Paragraph(this.text);

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 8),
      child: Text(
        text,
        style: Theme.of(context).textTheme.bodyMedium,
      ),
    );
  }
}

class _Bullet extends StatelessWidget {
  final String text;
  const _Bullet(this.text);

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 4),
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          const Text('· '),
          Expanded(
            child: Text(
              text,
              style: Theme.of(context).textTheme.bodyMedium,
            ),
          ),
        ],
      ),
    );
  }
}