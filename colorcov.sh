rm all.log allcov.log allcovnum.log
cat klee-last/*.cov >all.log
uniq all.log>allcov.log
cat allcov.log|awk -F: '{print $2}' >allcovnum.log
python colorcov.py|less
