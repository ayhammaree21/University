from socket import *
from food import food

foodList = []
PORT = 6500
# defining the socket, and binding it to the port
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', PORT))
# socket listening for response
serverSocket.listen(1)

print('The server is ready to receive')


# a function to read the food csv file
def readFile(fileName):
    foodList.clear()
    with open(fileName) as f:
        line = " "
        next(f)  # To ignore reading the first line on the csv file (header line)
        while line:
            line = f.readline()

            if line != "":
                lineSplit = line.split(';')  # splitting the csv file lines by commas to get the desired data

                Food = food(lineSplit[0], lineSplit[1])
                foodList.append(Food)  # putting the file information in a list


# return name function to use as a key in the list sort function
def retName(Food):
    return Food.name


# return name function to use as a key in the list sort function
def retPrice(Food):
    return float(Food.price)


# sort the list by name after storing the file data in it
def sortByName():
    readFile('foodlist.csv')
    foodList.sort(key=retName)


# sort the list by price after storing the file data in it
def sortByPrice():
    readFile('foodlist.csv')
    foodList.sort(key=retPrice)


while True:
    connection, add = serverSocket.accept()
    sentence = connection.recv(1024).decode('utf-8')

    requesting_file = sentence.split(' ')[1]  # from the request sentence, getting the requested file
    requestedFile = requesting_file.lstrip('/')  # removing the first / to get the requested file name

    if requestedFile == '' or requestedFile == "index.html":  # default request
        requestedFile = 'main.html'  # Load main.html file as default

    try:
        sortedBy = ''

        # accepting different file formats
        if requestedFile.endswith(".jpg"):
            requestedType = 'image/jpg'

        elif requestedFile.endswith(".jpeg"):
            requestedType = 'image/jpeg'

        elif requestedFile.endswith(".png"):
            requestedType = 'image/png'

        elif requestedFile.endswith(".css"):
            requestedType = 'text/css'

        elif requestedFile.upper() == "SORTBYNAME":
            sortByName()
            sortedBy = 'Name'
            requestedType = 'text/html'

        elif requestedFile.upper() == "SORTBYPRICE":
            sortByPrice()
            sortedBy = 'Price'
            requestedType = 'text/html'

        else:
            requestedType = 'text/html'

        if requestedFile.upper() != "SORTBYNAME" and requestedFile.upper() != "SORTBYPRICE":
            file = open(requestedFile, 'rb')  # opening the requested file
            response = file.read()  # reading the file
            file.close()  # closing the file

        else:
            response = ('<!DOCTYPE html><html><head><title>Food Prices</title><style type="text/css">.header {'
                        'width:100%;height: 85px;background-color: #ffbf00;padding: 10px 0 0 20px;border: '
                        '0px;border-radius:0px;}table {font-family: arial, sans-serif;border-collapse: '
                        'collapse;width: 100%;}td, th {border: 1px solid #e6ac00;text-align: center;padding: '
                        '8px; width: 50%; font-weight: normal;}tr:nth-child(even) {background-color: '
                        '#fff2cb;}</style></head><body><div class="header"><h1 style="color: black;">Food '
                        'Prices</h1></div><p>Food Prices Sorted By ' + sortedBy + '</p><table><tr><th style="border: '
                        '1px solid black; font-weight: bold;">Food</th><th style="border: 1px solid black; '
                        'font-weight: bold;">Price</th></tr>').encode()
            for item in foodList:
                response += ('<tr><th>' + str(item.name) + '</th><th>' + str(item.price) + '</th></tr>').encode()
            response += '</table></body></html>'.encode()

        header = 'HTTP/1.1 200 OK\r\n'  # the first part of the header to send.
        header += 'Content-Type: ' + str(requestedType) + '\r\n\r\n'

    except Exception as e:  # Exception if the request the user has entered doesn't exist
        header = 'HTTP/1.1 404 Not Found\r\n'
        header += 'Content-Type: text/html\r\n\r\n'
        response = '<!DOCTYPE html><html><head><title>Error</title><style type="text/css">h1 {text-align: center;}li ' \
                   '{font-weight: bold;}</style></head><body><h1 style="color:red">The file is not ' \
                   'found</h1><br><ul><li> Ayham Maree 1191408</li><li> Sara Ammar 1191052</li><li> Alaa ' \
                   'Sehwail 1191741</li><li> Mahmoud Shoman 1191648</li></ul><div style="position: relative; top: 120px;"><p>Client IP: ' + \
                   str(add[0]) + '</p><p>Client PORT: ' + str(add[1]) + '</p></div></body></html>'
        response = response.encode()

    final_response = header.encode() + response  # encoding the header and adding the response to the request
    connection.send(final_response)  # sending the final response with all parts of header
    connection.close()
    print(sentence)  # Print the HTTP request on the terminal window