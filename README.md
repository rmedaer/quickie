# Quickie

This small Python runtime wrapper automatically set capabilities to bind IP ports under 1024.
Therefore you don't need to be root to run Python module listening on these protected ports.

## Motivation

In most of my projects the development setup try to be as close as possible to the production environment.
I often write a small Python script which run all the services needed for the project. Including a web server
listening on ports 80 and 443. However I need "root" permissions to bind these ports. To avoid running
development services as root, I had to add `cap_net_bind_service` capabilities to Python.

Still not good enough! So I created this small piece of software: **quickie**, _delivers a lot of fun
in a very short time..._ It's basically a software which run Python module configured in your `.quickieconfig` file.

## How-to

**Wanna try a quickie** ? Quiet simple! After build & installation, you just have to write a `.quickieconfig` file
at root of your projet. Inside this file you put the Python module to run and optionally Python paths to add.

For instance:
```
[main]
module = my.module
```

Path of `.quickieconfig` file is automatically added in `sys.path`.

Then just run `quickie` any where in your project. It does the equivalent of `python3 -m my.module`.

It might ask you to set its `cap_net_bind_service` capability:

```
sudo setcap "cap_net_bind_service+ep" quickie
```

## Build and install

Autotools is used to configure and make the project. Business as usual:

```
./autogen.sh
./configure
make
make install
```

## License

Licensed under Apache-2.0
