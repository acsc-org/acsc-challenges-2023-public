FROM dart:2.18.6@sha256:33caaa23adc28949959517733f989e86f077ea6dee825d543685df1fcf5d57f1

RUN apt update -y

COPY ./distfiles/pdf_converter /app

WORKDIR /app
RUN echo "ACSC{WTF even php fixed this behavior!b907af1e}" > ./flag
RUN adduser --system --group web
RUN chown -R web:web .
USER web:web
RUN dart pub get
RUN dart pub get --offline

CMD ["dart", "run"]

EXPOSE 8080
