#!/bin/bash
make
sudo cp -rf . /opt/TestSDL/
sudo ln -s /opt/TestSDL/bin/TestSDL /usr/local/bin/TestSDL
echo ' -> Installation réussie !'
