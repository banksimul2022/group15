#!/bin/sh

target_file=".env"

[ -f "${target_file}" ] && sed '/^JWT_SECRET=".*"$/d' -i "${target_file}"
echo "JWT_SECRET=\"$(openssl rand -hex 64)\"" > "${target_file}"
