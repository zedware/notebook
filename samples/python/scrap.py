from pip._vendor import requests

session = requests.Session()
params = {'username': 'username', 'password': 'password'}
s = session.post('http://pythonscraping.com/pages/cookies/welcome.php', params)
print(s.text)
print('Cookie')
print(s.cookies.get_dict())
s = session.get('http://pythonscraping.com/pages/cookies/profiles.php')
print(s.text)
print('Done')

# from pip._vendor import requests
#
# session = requests.Session()
# params = {'username': 'username', 'password': 'password'}
# s = session.post('http://www.bing.com', params)
# print(s.text)
# print('Cookie')
# print(s.cookies.get_dict())
# s = session.get('http://pythonscraping.com/pages/cookies/profile.php')
# print('Done')
