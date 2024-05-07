import pickle
import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer

# class MyClassifier:
#
#     def __init__(self, model_file, vectorized_file_1, vectorized_file_2):
#         with open(model_file, 'rb') as handle:
#             self.loaded_model = pickle.load(handle)
#
#         with open(vectorized_file_1, 'rb') as handle:
#             self.loaded_vectorized_1 = pickle.load(handle)
#
#         with open(vectorized_file_2, 'rb') as handle:
#             self.loaded_vectorized_2 = pickle.load(handle)
#
#     def predict(self, new_data):
#         # Create a new instance of the vectorized and set its settings to match the saved vectorized
#         self.vectorized_1 = CountVectorizer(analyzer='word', lowercase=False, vocabulary=self.loaded_vectorized_1.vocabulary_)
#
#         # Transform the new data using the saved vectorized
#         emoji_feature = self.vectorized_1.fit_transform(new_data).toarray()
#
#         # Create a new instance of the vectorized and set its settings to match the saved vectorizer
#         self.vectorized_2 = CountVectorizer(analyzer='word', lowercase=False,
#                                             vocabulary=self.loaded_vectorized_2.vocabulary_)
#         # Transform the new data using the saved vectorized
#         tweet_feature = self.vectorized_1.fit_transform(new_data).toarray()
#
#         new_features = pd.concat([tweet_feature, emoji_feature], axis=1)
#
#         # Make predictions using the loaded model
#         predictions = self.loaded_model.predict(new_features)
#
#         return predictions

class MyClassifier:

    def __init__(self, model_file, vectorized_file):
        with open(model_file, 'rb') as handle:
            self.loaded_model = pickle.load(handle)

        with open(vectorized_file, 'rb') as handle:
            self.loaded_vectorized = pickle.load(handle)



    def predict(self, new_data):
        # Create a new instance of the vectorized and set its settings to match the saved vectorized
        self.vectorized = CountVectorizer(analyzer='word', lowercase=False, vocabulary=self.loaded_vectorized.vocabulary_)

        # Transform the new data using the saved vectorized
        new_features = self.vectorized.fit_transform(new_data).toarray()

        # Make predictions using the loaded model
        predictions = self.loaded_model.predict(new_features)

        return predictions


