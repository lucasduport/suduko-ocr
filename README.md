<h1>OCR Project | S3 Epita</h1>

<p>This is our third semester project at EPITA. You can find the subject
<a href="https://cdn.discordapp.com/attachments/1002568563892166838/1037486340301918240/ocr_sudoku_solver_fr.pdf">here</a>.</p>

<h2>Getting Started</h2>

<p>These instructions will give you a copy of the project up and running on
your local machine for development and testing purposes. See deployment
for notes on deploying the project on a live system.</p>

<h3>Prerequisites</h3>

<p>Requirements for the software and other tools to build, and test: <br/>
- <a href="https://clang.llvm.org/">Clang</a> <br/>
- <a href="https://www.libsdl.org/">Simple DirectMedia Layer library (SDL)</a></p>

<h3>Installing</h3>

<p>A step by step series of examples that tell you how to get a development
environment running</p>

<p>First, you have to install clang compiler</p>

<pre><code>sudo apt install clang
</code></pre>

<p>Then, you have to install SDL2 library</p>

<pre><code>sudo apt install libsdl2-dev
</code></pre>

<p>Now, you have to install SDL2 image library</p>

<pre><code>sudo apt install libsdl2-image-dev
</code></pre>

<p>Finally, you have all the tools needed for the project.</p>

<h3>Correct indentation and coding style:</h3>

<pre><code>clang-format --style=file:.clang-format -i *.c
</code></pre>

<h2>NeuralNetwork/</h2>

<h3>Build</h3>

<p>Build for release</p>

<pre><code>make
</code></pre>

<p>Build for debug</p>

<pre><code>make debug
</code></pre>

<p>Clean build files</p>

<pre><code>make clean
</code></pre>

<p>Clean build files and executable</p>

<pre><code>make clear
</code></pre>

<p>Shortcut for Training demo; arg attempt=[int]</p>

<pre><code>make runTrain
</code></pre>

<p>Shortcut for Prediction demo; arg v1=[0|1] &amp; v2=[0|1]</p>

<pre><code>make runPredict
</code></pre>

<h3>Run</h3>

<ul>
<li><p>Training:</p>

<pre><code>./NeuralNetwork TrainXOR [Config file (*.cfg)] [Number of attempt]
</code></pre></li>
<li><p>Prediction:</p>

<pre><code>./NeuralNetwork PredictXOR [NeuralNetwork data (*.dnn)] [0|1] [0|1]
</code></pre></li>
</ul>

<h3>Examples:</h3>

<p>Example of train with 1000 attemps:</p>

<pre><code>ex: ./NeuralNetwork TrainXOR TrainedNetwork/NN.cfg 1000
</code></pre>

<p>Example for predicition:</p>

<pre><code>./NeuralNetwork PredictXOR TrainedNetwork/NeuralNetData_3layers_XOR_100.0.dnn 1 0
</code></pre>

<h2>ImageProcessing/</h2>

<h3>Build</h3>

<p>Build for release</p>

<pre><code>make
</code></pre>

<h3>Run</h3>

<p>Try --help or -h for more information</p>

<pre><code>./main --help
</code></pre>

<h3>Examples:</h3>

<pre><code>./main -d &lt;ImagePath&gt; # Demonstration
./main -r &lt;ImagePath&gt; &lt;Angle&gt; # Rotate image with angle
</code></pre>

<h2>Solver/</h2>

<h3>Build</h3>

<p>Build for release</p>

<pre><code>make
</code></pre>

<h3>Run</h3>

<pre><code>./solver &lt;Textfile&gt;
</code></pre>

<h3>Example:</h3>

<pre><code>./solver &lt;Textfile&gt; # Solve sudoku wich is in textfile
</code></pre>

<h2>Authors</h2>

<ul>
<li>Mattéo BAUSSART - 
<a href="matteo.baussart@epita.fr">matteo.baussart@epita.fr</a></li>
<li>Lucas DUPORT - 
<a href="lucas.duport@epita.fr">lucas.duport@epita.fr</a></li>
<li>Julie BLASSIAU - 
<a href="julie.blassiau@epita.fr">julie.blassiau@epita.fr</a></li>
<li>Matthieu CORREIA - 
<a href="matthieu.correia@epita.fr">matthieu.correia@epita.fr</a></li>
</ul>
