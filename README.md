<h1 align="center">🎵 MP3 Organizer CLI Tools</h1>

<p align="center">
  A lightweight collection of <b>C++ command-line utilities</b> to organize, manage, and customize your local MP3 albums or playlists
  (i.e. folders full of <code>.mp3</code> files).
</p>

<p align="center">
  This repo provides simple, standalone tools to <b>rename</b>, <b>delete</b>, and <b>sync</b> MP3 files — plus an optional utility for
  reading and editing MP3 metadata using 
  <a href="https://github.com/taglib/taglib" target="_blank">TagLib</a>.
</p>

<hr/>

<h2>📁 Repository Structure</h2>

<pre>
.
├── renameMp3.cpp        # Remove specified strings from mp3 filenames
├── deleteMp3.cpp        # Delete mp3 files based on rules or patterns
├── syncMp3.cpp          # Sync mp3 files between devices/folders
└── artistMp3/
    ├── artistMp3.cpp    # Uses TagLib to append artist to mp3 title
    └── CMakeLists.txt   # Build configuration for artistMp3 utility
</pre>

<hr/>

<h2>⚙️ Compilation</h2>

<p>
  Each script (except <code>artistMp3</code>) can be easily compiled using <code>g++</code>:
</p>

<pre>
g++ renameMp3.cpp -o renameMp3
</pre>

<pre>
g++ deleteMp3.cpp -o deleteMp3
</pre>

<pre>
g++ syncMp3.cpp -o syncMp3
</pre>

<p>Then execute them directly:</p>

<pre>
./renameMp3
./deleteMp3
./syncMp3
</pre>

<p>
  💡 These utilities are built to work locally — no external dependencies required
  (except <code>artistMp3</code>, which depends on TagLib and CMake).
</p>

<hr/>

<h2>🧠 Command Usage</h2>

<h3>🏷️ renameMp3</h3>

<p>Removes specified strings from within the filenames of MP3 files.</p>

<pre>
./renameMp3 -h
</pre>

<b>Usage:</b>
<pre>
./renameMp3 [-d directory] [<what to remove>]
    -h --help : display this help message
    -d --directory : specify the directory containing mp3 files (default is current directory)
    <what to remove> : specify the strings to remove from mp3 filenames
</pre>

<b>Example:</b>
<pre>
./renameMp3 -d ../SweetSorrow 'SpotiDown - ' Youtube 'Michael Jackson'
</pre>

<p>This will remove each of the following strings from all songs in the specified directory:</p>

<ol>
  <li><code>SpotiDown - </code></li>
  <li><code>Youtube</code></li>
  <li><code>Michael Jackson</code></li>
</ol>

<hr/>

<h3>🔁 syncMp3</h3>

<p>
  Synchronizes two albums (folders) on different devices — for example, between your
  <b>laptop</b> and <b>phone</b> — provided they are physically connected
  (e.g., one is mounted to another). It operates directly within the
  <code>/user/Music</code> directory.
</p>

<p>Ideal if you want to have the same downloaded playlist across devices.</p>

<pre>
g++ syncMp3.cpp -o syncMp3
./syncMp3
</pre>

<hr/>

<h3>🧩 artistMp3 (uses TagLib)</h3>

<p>
  This optional utility uses the 
  <a href="https://github.com/taglib/taglib" target="_blank">TagLib</a> library
  to <b>append artist names to MP3 titles</b> or modify metadata.
</p>

<p>Located in the <code>artistMp3/</code> directory:</p>

<pre>
artistMp3/
├── artistMp3.cpp
└── CMakeLists.txt
</pre>

<h4>📦 Dependency Setup</h4>

<p>
  To use this utility, you must first install <b>TagLib</b> — a C++ library for reading and editing audio metadata.
</p>

<p>
  ⚠️ <b>Note:</b> TagLib itself requires <b>CMake</b> to be installed in order to build and install correctly.
</p>

<p>
  Follow the official TagLib installation instructions here:  
  👉 <a href="https://github.com/taglib/taglib/blob/master/INSTALL.md" target="_blank">TagLib Installation Guide</a>
</p>

<h4>🔧 Compilation (CMake Only)</h4>

<p>
  <code>artistMp3</code> <b>can only be compiled using CMake</b>, as it depends on TagLib’s CMake configuration.
</p>

<pre>
cd artistMp3
cmake .
make
</pre>

<p>This will generate the <code>artistMp3</code> executable in the same directory.</p>

<h4>🪄 Usage</h4>

<pre>
./artistMp3 -h
</pre>

<b>Help output:</b>
<pre>
For all specified artists, add artist to mp3 title
USAGE: ./artistMp3 [ -f FILES] [ -a ARTISTS]
OPTIONS:
  -h, --help      Show this help message
  -d, --dir       Specify the path to search
                  (default is current directory)
  -f, --files     Specify files to process (.mp3)
                  (default is all files in directory per the specified artists)
  -s, --sep       Specify separator between title and artist
                  (default is " - ")
  -a, --artists   Specify artists to process
                  (default is all artists found in specified files)
  --total         Select EVERY artist and EVERY file in path
                  (incompatible with FILES and ARTISTS arguments)
</pre>

<hr/>

<h2>💻 Requirements</h2>

<ul>
  <li><b>C++17 or newer</b></li>
  <li><b>g++ compiler</b></li>
  <li><b>CMake</b> (required for building TagLib and <code>artistMp3</code>)</li>
  <li><a href="https://github.com/taglib/taglib" target="_blank">TagLib</a> (for <code>artistMp3</code> only)</li>
</ul>

<hr/>

<h2>📜 License</h2>

<p>
  This project is released under the <b>MIT License</b>.<br/>
  You are free to use, modify, and distribute it — just include a copy of the license.
</p>

<hr/>

<h2>🤝 Contributions</h2>

<p>
  Pull requests, improvements, and new CLI tools are welcome!<br/>
  If you’d like to extend the repo with new functionality, feel free to fork and submit a PR.
</p>
