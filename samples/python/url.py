from urllib.request import urlopen
from urllib.error import HTTPError
from bs4 import BeautifulSoup


def get_title(url):
    try:
        html = urlopen(url)
    except HTTPError as e:
        return None

    try:
        # print(html.read().decode('utf-8'))
        bs = BeautifulSoup(html.read(), 'html.parser')
        title = bs.body.h1
    except AttributeError as e:
        return None

    return title


gTitle = get_title('https://zedware.github.io')
if gTitle is None:
    print('Title not found')
else:
    print(gTitle)
