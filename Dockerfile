FROM alpine:latest
LABEL author="lmriccardo"

RUN apk update
RUN apk add --no-cache build-base make sudo bsd-compat-headers

WORKDIR /cnetip/

CMD [ "make" ]