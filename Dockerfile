#Docker EverGreenCoin daemon RC-1
#Stage 1 The daemon builder
FROM ubuntu:18.04 AS builder
WORKDIR /home/root
RUN ["apt", "update"]
RUN ["apt", "install", "git", "-y"]
#Get the Stage 1 Builder script that builds the daemon
RUN ["git", "clone", "https://gist.github.com/xbrooks/44e375d582ffd4c1b7d7dbf61ad39435"]
RUN ["chmod", "+x", "44e375d582ffd4c1b7d7dbf61ad39435/build-install-daemons.sh"]
RUN ["44e375d582ffd4c1b7d7dbf61ad39435/build-install-daemons.sh", "egc", "https://github.com/evergreencoindev/evergreencoin", "evergreencoind", "evergreencoin"]

#Get the Stage 2 Configurator script here so the next stage doesn't need git
RUN ["git", "clone", "https://gist.github.com/xbrooks/f142b7ed83d8b0adab9b27d71145b94c"]
#Also get the Container Runtime Configurator that generates wallet and writes the conf
RUN ["git", "clone", "https://gist.github.com/xbrooks/05392718769be46086514f839e57c137"]
RUN ["chmod", "+x", "/home/root/f142b7ed83d8b0adab9b27d71145b94c/egc-docker-configure.sh", "/home/root/05392718769be46086514f839e57c137/run-egc.sh"]

#Stage 2. Just Ubuntu and the daemon (and its runtime deps)
FROM ubuntu:18.04
COPY --from=builder /home/egc/evergreencoind /home/root/f142b7ed83d8b0adab9b27d71145b94c/egc-docker-configure.sh /home/root/05392718769be46086514f839e57c137/run-egc.sh /home/root/
RUN ["/home/root/egc-docker-configure.sh"]
#P2P port: 5757 (testnet 15757)
#RPC port: 5758 (testnet 15758)
EXPOSE 5757/tcp 5758/tcp

CMD ["/usr/local/bin/run-egc.sh", "egc", "evergreencoind", "evergreencoin"]
