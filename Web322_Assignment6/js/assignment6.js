// set a global httpRequest object

var httpRequest;
		
// DONE: when the page (window) has loaded, make a
// request for page 1
window.onload = makeRequest(1);



function makeRequest(pageNum) {
	
    // DONE: create a variable "url" to store the request to 
	// the current pageNum, ie:
	//
	// "https://reqres.in/api/users?page=1" // for page 1
	// "https://reqres.in/api/users?page=2" // for page 2
	// etc...
	var url = "https://reqres.in/api/users?page="+pageNum;
	
	// make an HTTP request object
	
	httpRequest = new XMLHttpRequest();

	// execute the "showContents" function when
	// the httprequest.onreadystatechange event is fired
	
	httpRequest.onreadystatechange = showContents;
	
	// open a asynchronous HTTP (GET) request with the specified url
	
	httpRequest.open('GET', url, true);
	
	// send the request
	
	httpRequest.send();
}

// the function that handles the server response

function showContents() {

//  check for response state
//  0      The request is not initialized
//  1      The request has been set up
//  2      The request has been sent
//  3      The request is in process
//  4      The request is complete

	if (httpRequest.readyState === 4) {
		// check the response code
		if (httpRequest.status === 200) { // The request has succeeded
		// Javascript function JSON.parse to parse JSON data
			
			var jsData = JSON.parse(httpRequest.responseText);
			
			// DONE: use the jsData object to populate the correct
			// table cells, ie <tr><td>...</td></tr>
			// in the <tbody> element with id="data"
			document.getElementById("data").innerHTML = "";
			var table = document.getElementById("data");
				for(var i=0; i< jsData.per_page; i++){
				var row = document.createElement("tr");//create three rows
				var data=[jsData.data[i].id,jsData.data[i].first_name,jsData.data[i].last_name];
					for(var j=0;j<jsData.data.length; j++){//create four columns
				
				var column = document.createElement("td");
				column.appendChild(document.createTextNode(data[j]));
				var avatar = document.createElement("img");
				avatar.appendChild(document.createTextNode(jsData.data[j].avatar));
				avatar.setAttribute("src",jsData.data[i].avatar);
				var avatarcolumn = document.createElement("td");
				avatarcolumn.appendChild(avatar);
				row.appendChild(column);
			}
			row.appendChild(avatarcolumn);
			table.appendChild(row);
		}

			// DONE: remove the class "active" from all elements with the class "pgbtn"
			
			for(var i = 1;i < 5;i++){
				if(jsData.page == i){
				document.getElementById("pgbtn"+i).className = 'active';
				}else if(jsData.page != i){
					document.getElementById("pgbtn"+i).className = 'inactive';
				}
			}
			// DONE: add the class "active" to the button corresponding to the active page, ie:
			// if jsData.page is 1, add the class "active" to button element with id pgbtn1
			// if jsData.page is 2, add the class "active" to button element with id pgbtn2
			// etc...
		} else {
			console.log('There was a problem with the request.');
		}
	}
}	
