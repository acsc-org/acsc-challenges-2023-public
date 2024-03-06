import 'dart:io';
import 'dart:async';
import 'dart:isolate';
import 'dart:typed_data';
import 'package:shelf/shelf.dart';
import 'package:shelf/shelf_io.dart' as shelf_io;
import 'package:shelf_router/shelf_router.dart';
import 'package:shelf_multipart/form_data.dart';
import 'package:shelf_multipart/multipart.dart';
import 'package:path/path.dart' as p;
import 'package:pdf/pdf.dart';
import 'package:pdf/widgets.dart' as pw;
import 'package:image/image.dart' as im;

Stream<List<int>> fileIterator(RandomAccessFile f) async* {
  while (true) {
    try {
      final b = await f.read(1024).timeout(const Duration(seconds: 3));
      if (b.length == 0) return;
      yield b;
    } on TimeoutException {
      return;
    }
  }
}

Future<List<int>> getFileContent(String fname, Response? resp) async {
  List<int> content = [];

  final fpath = p.join('assets', fname);
  if (!p.isWithin('assets', p.normalize(fpath))) {
    resp = Response.badRequest(body: 'Hello, hacker!');
    return content;
  }

  if (!File(fpath).existsSync()) {
    resp = Response.badRequest(body: 'You see non-existent files?');
    return content;
  }

  final f = File(fpath).openSync();
  final completer = Completer<void>();
  final stream = fileIterator(f);
  final subscription = stream.listen(
    (byte) {
      content.addAll(byte);
    },
    onDone: () => completer.complete()
  );
  try {
    await completer.future.timeout(const Duration(seconds: 3));
  } on TimeoutException {
  }
  subscription.cancel();
  f.close();

  return content;
}

class Routing {
  Handler get handler {
    final router = Router();

    router.get('/', (Request request) async {
      return Response.ok(
                File('index.html').readAsStringSync(),
                headers: {
                  'Content-Type': 'text/html',
                }
            );
    });

    router.post('/', (Request request) async {
      String image_fname = '';
      List<int> image_content = [];

      final names = <String, String?>{};
      final params = <String, List<int>>{};
      await for (final formData in request.multipartFormData) {
        names[formData.name] = formData.filename;
        params[formData.name] = await formData.part.readBytes();
      }

      if (params.containsKey('rawfile')) {
        image_fname = names['rawfile'] ?? '';
        image_content = params['rawfile'] ?? [];
      } else {
        image_fname = String.fromCharCodes(params['image-presets'] ?? []);
        Response? resp = null;
        image_content = await getFileContent(image_fname, resp);
        if (resp != null) return resp;
      }

      final head_hexdump = image_content.take(64)
          .map((int b) {
             var s = b.toRadixString(16);
             if (s.length == 1) s = '0' + s;
             return s;
          }).join(' ');
      print('Image to convert: $head_hexdump');

      final ext = p.extension(image_fname).toLowerCase();

      pw.Widget image_widget;
      if (ext == '.svg') {
        image_widget = pw.SvgImage(svg: String.fromCharCodes(image_content));
      } else {
        im.Image image = im.Image(0, 0);
        switch (ext.toLowerCase()) {
          case '.jpg':
          case '.jpeg':
            image = im.decodeJpg(image_content) ?? image;
            break;
          
          case '.png':
            image = im.decodePng(image_content) ?? image;
            break;

          case '.gif':
            image = im.decodeGif(image_content) ?? image;
            break;

          case '.ico':
            image = im.decodeIco(image_content) ?? image;
            break;

          case '.bmp':
            image = im.decodeBmp(image_content) ?? image;
            break;

          default:
            return Response.badRequest(body: 'Invented a new file format?');
        }
        image_widget = pw.Image(pw.ImageImage(image));
      }

      final pdf = pw.Document();
      pdf.addPage(pw.Page(build: (pw.Context context) {
         return pw.Center(child: image_widget);
      }));

      return Response.ok(
                await pdf.save().timeout(const Duration(seconds: 10)),
                headers: {
                  'Content-Type': 'application/pdf',
                }
             );
    });

    router.all('/<ignored|.*>', (Request request) {
      return Response.notFound('No such a page');
    });

    return router;
  }
}

void startServer(arg) async {
  final routing = Routing();
  final server = await HttpServer.bind('0.0.0.0', 8080, shared: true);
  shelf_io.serveRequests(server, routing.handler);
}

void main() async {
  final N = 10;
  for (var i=1; i<N; i++) {
    Isolate.spawn(startServer, null);
  }

  startServer(null);

  print('Server started');
  await ProcessSignal.sigterm.watch().first;
}
