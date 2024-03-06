FROM ubuntu:22.04

RUN apt-get -y update --fix-missing
RUN apt-get -y upgrade
RUN apt-get -y install socat
RUN groupadd -r ctf && useradd -r -g ctf ctf

WORKDIR /home/ctf
RUN echo "ACSC{snuk3_vs_vu1ner4bi1ity_58a2552fea58}" > flag.txt
COPY distfiles/snukebird .
COPY distfiles/stage01.txt .
COPY distfiles/stage02.txt .
COPY distfiles/stage03.txt .
RUN chmod -R og-w .

RUN chown -R root:ctf /home/ctf

USER ctf
CMD socat TCP-L:7441,reuseaddr,fork EXEC:"./snukebird"
