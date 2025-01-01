{
  description = "Java";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  # Flake outputs
  outputs = { self, nixpkgs, ...}@inputs:
    let
	    system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
    in
    {

		devShells.${system}.default = pkgs.mkShell {
			name = "Graphics";
			buildInputs = with pkgs;[
				zulu
				jdt-language-server
			];
		};
	};
}
