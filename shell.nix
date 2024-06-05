{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.raylib
    pkgs.gcc
    pkgs.cmake
  ];
}
