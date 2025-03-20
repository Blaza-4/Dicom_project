
import requests
import os
import time
from bs4 import BeautifulSoup
import json

global data
with open('data.json') as json_file:
     data = json.load(json_file)

print("operation")
jj=open("data.json","w")
json.dump(data,jj)
jj.close()
print("data upgraded","|| data length:",len(data))
print("total questions:",len(data))


r=requests.get("https://www.usa.philips.com/healthcare/product/HCNMRF429/mr-7700")
#r=requests.get("https://www.usa.philips.com")
soup=BeautifulSoup(r.content, 'html.parser')
l=soup.find_all("title")

m = soup.find_all("div", class_=["s-prose js-post-body","container","p-grid-item"])
print(f"Total elements found: {len(m)}")
if m==[]:
    print("pass")
    pass
else:
    for i in range(len(m)):
      print(m[i].text.strip())
      with open("last.txt","a")as f:
        f.write(m[i].text.strip() + "\n")
        f.close()
print("done")
     


