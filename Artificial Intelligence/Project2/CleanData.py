import re
import emoji
from nltk.tokenize import word_tokenize
from nltk.corpus import stopwords
from nltk.stem import SnowballStemmer

def extract_emojis(s):
  return ''.join(c for c in s if c in emoji.UNICODE_EMOJI['en'])


def set_emoji_feature(data):
    data['Emoji'] = data['Tweets'].apply(extract_emojis)
    return data


def clean_tweet(tweet):
    # Remove URLs, RTs, and twitter handles
    tweet = re.sub(
        r'(?i)\b((?:https?://|www\d{0,3}[.]|[a-z0-9.\-]+[.])+[a-z]{2,4}(?:[^\s()<>]+|\(([^\s()<>]+|(\([^\s()<>]+\)))*\))+(?:\(([^\s()<>]+|(\([^\s()<>]+\)))*\)|[^\s`!()\[\]{};:\'".,<>?«»“”‘’]))',
        '', tweet)
    tweet = re.sub(r'RT @[\w_]+:', '', tweet)
    tweet = re.sub(r'@[\w_]+', '', tweet)

    # Tokenize the text
    tokens = word_tokenize(tweet)

    # Remove stop words and punctuation
    stop_words = set(stopwords.words("arabic"))
    filtered_tokens = [word for word in tokens if word.isalnum() and word not in stop_words]

    # Stem the tokens
    stemmer = SnowballStemmer("arabic")
    stemmed_tokens = [stemmer.stem(word) for word in filtered_tokens]

    # Join the cleaned tokens
    cleaned_tweet = ' '.join(stemmed_tokens)

    cleaned_tweet+=' '

    return cleaned_tweet



