import nltk

text1 = 'hello he heloo hello hi '
text1 = text1.split(' ')
fdist1 = nltk.FreqDist(text1)
#Get 50 Most Common Words
print (fdist1.most_common(50))''''
