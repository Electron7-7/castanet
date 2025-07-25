# castanet
A fun little web-scraper, that doesn't discriminate.

## Note for Windows Users
I haven't tested the pre-compiled .exe files on Windows, so your mileage may vary. I'm also not paying to sign my code, so you'll probably wanna compile it yourself using MSYS2, anyways (otherwise, Windows Defender throws a huge fit).

## What does it do?
Castanet started life as a simple script, first in Bash, then in Python, and even though it's now a fully fledged C++20 program... it's not that much more complex. The main function of castanet is running `nmap`; yeah, that's, like, 80% of the entire thing. The other 20% is formatting the output and storing it in a file. The magic comes from *how* castanet runs `nmap`. When I first wrote the little bugger back in ~2021-2022, I picked out a few ports that had potential for hiding some juicy secrets, and made a script that would let me pick any number of random hosts to scan, checking if any of those juicy ports were open. Not much has changed, not even the port choices; castanet is designed to be a very small program that slowly, over time, can build you a massive list of IPv4 addresses (and even DNS names!) that might be fun to poke at, so why not build yourself a phonebook of interesting computers, close your eyes, and see where your finger lands?

Cast yourself a little net over... well, the net!

## What's next?

I'd like to add a few quality of life changes, including:
- A status bar/message while `nmap` is running
- The ability to change the `nmap` command
    - Either via environment variable, program flags, or both
- Maybe an option to use `-Pn` with `sudo`, for better results
- More colors???...
