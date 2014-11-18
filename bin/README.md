DongDu
===
DongDu is Vietnamese word segmention.

# Usage (Command line tool)
It supports a command-line interface.


## To segment a plain text file:
```
$./predictor -i input_file_path -o output_file_path
```
with:
* `input_file_path`: path to the input plain text.
* `output_file_path`: path to the output file.


## Use Machine Learning:
```
$./learner -i corpus_link -o model&map_folder_path -w window_length
```
with:
* `corpus_link`: path to the corpus(training data)
* `model&map_folder_path`: path to the folder where model file and map file will save in.
* `window_length`: read papers of DongDu.
