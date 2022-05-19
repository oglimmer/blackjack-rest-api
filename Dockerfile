FROM debian:11-slim

RUN apt update && apt install -y cmake g++ git

RUN cd /home && git clone https://github.com/oatpp/oatpp.git --depth=1 && cd oatpp/ && mkdir build && cd build && cmake .. && make install

COPY . /home/blackjack

RUN cd /home/blackjack && mkdir -p build && cd build && rm -rf * && cmake .. && make && cp blackjack-exe /usr/bin

CMD blackjack-exe