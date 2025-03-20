import requests
import os
import time
from bs4 import BeautifulSoup
import json

global last
last=int(open("last","r").read())

global data
with open('data.json') as json_file:
     data = json.load(json_file)




for n in range(last ,2000000):
    op=open('last','w')
    op.write(str(n))
    op.close()
    if len(data)%5==0:

       jj=open("data.json","w")
       json.dump(data,jj)
       jj.close()
       print("data upgraded","|| data length:",len(data))
       print("total questions:",len(data))

    r=requests.get("https://stackoverflow.com/questions/"+str(n))
    soup=BeautifulSoup(r.content, 'html.parser')
    l=soup.find_all("title")
    m=soup.find_all("div", {"class": "s-prose js-post-body"})
    title=l[0].text
    if m==[]:
       pass
    else:
         answer=m[1].text
         if "Page not found - Stack Overflow" in title:
            pass
         else:
              title=title.replace(" - Stack Overflow","")

              data.update({title:answer})
              print(str(n),"Done")




    time.sleep(5)

 


# # import requests
# # import os
# # import time
# # import json
# # from bs4 import BeautifulSoup


# # if os.path.exists("last"):
# #     with open("last", "r") as f:
# #         last = int(f.read().strip())
# # else:
# #     last = 1  
# # if os.path.exists("data.json"):
# #     with open("data.json", "r") as json_file:
# #         try:
# #             data = json.load(json_file)
# #         except json.JSONDecodeError:
# #             data = {}
# # else:
# #     data = {}
# # for n in range(last, 2_000_000):

# #     with open("last", "w") as f:
# #         f.write(str(n))

# #    #  if len(data) % 5 == 0:
# #         with open("data.json", "w") as json_file:
# #             json.dump(data, json_file, indent=4)
# #         print(f"Data upgraded || data length: {len(data)}")

# #     url = ("https://stackoverflow.com/questions/"+str(n))
# #    # url = ("https://baltgroup.com/")
# #     try:
# #         r = requests.get(url, timeout=10)
# #         r.raise_for_status() 
# #     except requests.RequestException as e:
# #         print(f"Skipping {n} due to error: {e}")
# #         time.sleep(5)  # Wait and retry next
# #         continue

    
# #     soup = BeautifulSoup(r.content, 'html.parser')
# #     title_tag = soup.find("title")
# #     answer_divs = soup.find_all("div", {"class": "s-prose js-post-body"})

# #     # Extract title
# #     if title_tag:
# #         title = title_tag.text.replace(" - Stack Overflow", "")
# #     else:
# #         continue  

# #     if answer_divs and len(answer_divs) > 1:
# #         answer = answer_divs[1].text.strip()
# #         data[title] = answer
# #         print(f"Scraped {n}: {title}")

# #     time.sleep(20)  



# # import requests
# # import os
# # import time
# # import json
# # import random
# # from bs4 import BeautifulSoup

# # # Load last processed question ID
# # if os.path.exists("last"):
# #     with open("last", "r") as f:
# #         last = int(f.read().strip())
# # else:
# #     last = 1  

# # # Load existing data
# # if os.path.exists("data.json"):
# #     with open("data.json", "r") as json_file:
# #         try:
# #             data = json.load(json_file)
# #         except json.JSONDecodeError:
# #             data = {}
# # else:
# #     data = {}


# # USER_AGENTS = [
# #     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36",
# #     "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.5359.124 Safari/537.36",
# #     "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.0.0 Safari/537.36"
# # ]

# # # Start scraping
# # for n in range(last, 2_000_000):
# #     with open("last", "w") as f:
# #         f.write(str(n))

# #     url = f"https://stackoverflow.com/questions/{n}"

# #     headers = {
# #         "User-Agent": random.choice(USER_AGENTS)
# #     }

# #     try:
# #         r = requests.get(url, headers=headers, timeout=10)
# #         r.raise_for_status()  
# #     except requests.RequestException as e:
# #         print(f"Skipping {n} due to error: {e}")
# #         time.sleep(20)  
# #         continue

# #     soup = BeautifulSoup(r.content, 'html.parser')
# #     title_tag = soup.find("title")
# #     answer_divs = soup.find_all("div", {"class": "s-prose js-post-body"})

# #     if title_tag:
# #         title = title_tag.text.replace(" - Stack Overflow", "")
# #     else:
# #         continue  

# #     if answer_divs and len(answer_divs) > 1:
# #         answer = answer_divs[1].text.strip()
# #         data[title] = answer
# #         print(f"Scraped {n}: {title}")

# #         # Save data after every question
# #         with open("data.json", "w") as json_file:
# #             json.dump(data, json_file, indent=4)

# #     time.sleep(random.randint(20, 30))  # Randomized delay to avoid blocking


# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.support.ui import WebDriverWait
# from selenium.webdriver.support import expected_conditions as EC
# from bs4 import BeautifulSoup

# driver = webdriver.Chrome()
# driver.get("https://www.usa.philips.com/healthcare")

# # Wait for the element to appear
# WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.CLASS_NAME, "p-grid-item")))

# html = driver.page_source
# soup = BeautifulSoup(html, "html.parser")

# #m = soup.find_all("div", {"class": "p-grid-item"})
# m = soup.find_all("div", class_=["genericheaderpage basepage page","p-header-follow-content"])
# print(f"Total elements found: {len(m)}")
# lng = len(m)

# for i in range(len(m)):
#     print(m[i].text.strip())
#     with open("last.txt","a") as f:
#       f.write(m[i].text.strip()+"\n")
#       print(lng)
    
# driver.quit()

