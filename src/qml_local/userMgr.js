function logIn(serverUrl, usrName, usrPswd, fingerprint, finished){
    if( usrName==="" || usrPswd==="") return;
    var xhr = new XMLHttpRequest()
    var url = serverUrl + "/admin.php?m=user&f=login"
    //url = encodeURI(url);
    //console.log(url)
    var data={}
    data.account = usrName
    data.password = Qt.md5( usrPswd + "1234" )
    data.referer = encodeURI(url);
    data.Fingerprint = fingerprint
    var dataString = JSON.stringify(data)
    console.log( dataString )
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.send( dataString );
    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.HEADERS_RECEIVED){
            console.log(xhr.getAllResponseHeaders())
        }
        if (xhr.readyState === XMLHttpRequest.DONE) {
            console.log(xhr.responseText)
            finished(xhr.responseText)
        }
    }
}
