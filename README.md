# archiveparse

archiveparse is a library implementing read-only access to the BSA archive
files used by The Elder Scrolls series, including Morrowind, Oblivion, Skyrim,
and Skyrim Special Edition. Fallout 3 and Fallout New Vegas are also expected
to work, but were not tested. Fallout 4 with its BA2 archives is not supported.

See archiveparse-test for an usage example.

Please note that currently the only supported platform is Windows, and that
loose file support is currently incomplete (lacks enumeration support).

# Building

archiveparse may be built using normal CMake procedures, and is generally
intended to included into an outer project as a submodule or by any other
means.

Please note that archiveparse uses git submodules, which should be retrieved
before building.

# Licensing

archiveparse is licensed under the terms of the MIT license (see LICENSE).

Note that archiveparse also includes a copy of zlib, which is licensed under 
its own license, and references lz4 as a submodule, which is licensed under 
two-clause BSD license.
