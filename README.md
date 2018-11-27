# micro-c
## Web microservices in c++

### What it is it
An exampleofhow to build microservices in c++
Theycommunicate with each other, and ask for data

### Features
- Built from scratch
- No external dependencies or external code
- Binaries don't need external libraries (only OS ones)
- Portable to BSD, Linux
- Comunication is HTTP
- Data uses a simplified YAML
- Since they communicate with HTTP, can be extended to run on different hosts

### Caveats
- Some parameters are hardcoded
- Binds only to localhost
- Webserver is not fully implemented
- Uses a simplified YAML, not fully compatible

### Build
Clone to a dir
Execute:
```
cmake .
```
Now you have a binary in each dir

### Run
Execute all programs:
```
./analysis/analysis
./invoicing/invoicing
./reporting/reporting
```
**Note**
Services don't go to background


### Client
`xclient` is both a client and **unit test** program.

For each of the parameters there is a differemt set of tests.

It inserts some data if needed.

Usage:
```
% ./client/xclient 
Usage: xclient analysis|invoicing|reporting
```
It is both a client

Use xclient invoicing several times to fill more data (duplicates etc):
% ./client/xclient 
% ./client/xclient 
% ./client/xclient 
```


### api
#### analysis
`/` Dummy controller - Post some data (> 10 chars) and get another result

`/abnormal/<client>` Finds abnormaol behaviour on the client
 
`/duplicates` Finds invoices with sane client and amount onthe last hour

`/same` Finds invoices issued by 2 clients at the same time


#### Invoicing
`/` Dummy controller - Post some data (> 10 chars) and get another result

`/create` Create a new invoice entry.
Send a POST request with (note leading space):
```
 client: 1
 items:42
 amount: 420.00
 
```

`/read/<id>` Reads all fields from invoice <id> 

`/update` Updates an invoice entry.
Send a PUT request with (note leading space):
```
 id: 3
 client: 3
 items:3
 amount: 30.00
```

`/delete/<id>` Deletes the invoice <id> . Use a DELETE request

`/all` Shows all invoices


#### Reporting
`/` Dummy controller - Post some data (> 10 chars) and get another result

`/all` Shows all invoices

`/client/<client>` Shows invoices for client <client>
 
`/last/<seconds>` Shows invoices for the last <seconds>

`/today` Shows invoices since midnight

