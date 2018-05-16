
/**
 * @brief     class for evb_websocket communication
 * 
 * @param     wsUri               String Uri with server IP and Port, Example "ws://10.0.0.2:7777"
 * @param     shared_inputs       Array of inputs (DOM) that are shared with evb
 */
var evb_websocket = function (wsUri, shared_inputs) {
	
	var m_shared_inputs = shared_inputs;
	
	var websocket = new WebSocket(wsUri);
        websocket.binaryType = "arraybuffer";
        websocket.onopen = function (evt) { private_onOpen(evt) };
        websocket.onclose = function (evt) { private_onClose(evt) };
        websocket.onmessage = function (evt) { private_onMessage(evt) };
        websocket.onerror = function (evt) { private_onError(evt) };
	
	this.onOpen;
	this.onClose;
	this.onStreamReceived;
	this.onError;
	this.onCommand;
	this.onUnrecognized;
	
        /**
         * @brief     Send own user command
         * 
         * @param     mess    Message
         */
	this.sendCommand = function (mess) {
		var message = "command:" + mess;
		websocket.send(message);
	}
	
	/**
         * @brief     Send raw string message
         * 
         * @param     mess    Message
         */
	this.sendRaw = function (mess)
	{
		websocket.send(mess);
	}
	
	/**
         * @brief     Function for RGB drawing, format is in OPENCV Mat 
         * 
         * @param     canvas       Canvas where buffer is drawing
         * @param     byteArray    Byte Array with RGB data
         * @param     imageWidth   Image Width
         * @param     imageHeight  Image Height 
         */
	this.drawRGBimage = function (canvas, byteArray, imageWidth, imageHeight) {
            var ctx = canvas.getContext('2d');

            var imageData = ctx.getImageData(0, 0, imageWidth, imageHeight); // total size: imageWidth * imageHeight * 4; color format BGRA
            var dataLen = imageData.data.length;
            
            var counter = 0;
            
            var dataLen = imageWidth * imageHeight;
                        
            var buf = new ArrayBuffer(imageData.data.length);
            var buf8 = new Uint8ClampedArray(buf);
            var data = new Uint32Array(buf);
            for (var i = 0; i < dataLen; i++) {

                data[i] =
                (255 << 24) |    // alpha
                (byteArray[counter + 1] << 16) |    // blue
                (byteArray[counter + 2] << 8) |    // green
                byteArray[counter];            // red
                counter += 3;
            }
            imageData.data.set(buf8);
            
            ctx.putImageData(imageData, 0, 0);
	};
	
        /**
         * @brief private Event handler when some shared input is changed
         */ 
	var onInputChange = function()
	{
            var message = "shared_update:";
            if (this.type === "checkbox")
                message += this.name + "&" + this.checked + "&";
            if (this.type === "number")
                message += this.name + "&" + this.value + "&";
            if (this.type === "radio") 
            {
                var radio = document.getElementsByName(this.name);

                for (var i = 0; i < radio.length; i++) {
                        if (radio[i].checked) {
                                message += this.name + "&" + radio[i].value + "&";
                        }
                }
            }
            websocket.send(message);
	}
	
	/**
         * @brief private Event handler when some websocket is opened
         */
        var private_onOpen = function(evt)
        {     
            var message = "shared_init:";  
            var index;
            
            for (index = 0; index < m_shared_inputs.length; ++index) {
                message += m_shared_inputs[index].name + "&";
                if (m_shared_inputs[index].type === "checkbox"){
                    message += m_shared_inputs[index].checked + "&";
                }
                else if (m_shared_inputs[index].type === "radio") {
                        radio = document.getElementsByName(m_shared_inputs[index].name);
                        for (var i = 0; i < radio.length; i++) {
                                if (radio[i].checked) {
                                        message += radio[i].value + "&";
                                }
                        }
                }
                else {
                    message += m_shared_inputs[index].value + "&";
                }		
                m_shared_inputs[index].onchange = onInputChange;
            }
                    
            onOpen(evt, message);
        }
      
       /**
         * @brief private Event handler when some websocket was closed
         */
        var private_onClose = function(evt)
        {
            onClose(evt);
            websocket.close();
        }
      
        /**
         * @brief   private Event handler when message is received
         * @details this select what type of message is received. Stream or own command or shared value. Depend on this apropriate event is triggered
         */
        var private_onMessage = function(evt)
        {
            // If packet is type of string
            if (typeof evt.data === "string") {
                var message = evt.data;
                if (message.startsWith("command:")){
                    message = message.slice("command:".length);
                    onCommand(message);
                }         
                else if (message.startsWith("connected:")) {
                    message = message.slice("connected:".length);
                    //connected.innerHTML = message;
                }
                else if (message.startsWith("shared:")) {
                    message = message.slice("shared:".length);
                    while (message.length>0)
                    {
                        var name = message.substring(0,message.indexOf("&"));
                        message = message.slice(message.indexOf("&") + 1);	 
                        var value = message.substring(0,message.indexOf("&"));
                        message = message.slice(message.indexOf("&") + 1);  
                        
                        var item = document.getElementsByName(name)[0];
                        if(!item) 
                        {
                                onUnrecognized(name, value);
                                continue;	
                        }
                            
                        
                        if (item.type === "checkbox") {
                                if (value === "true") {
                                        item.checked = true;
                                }
                                else {
                                        item.checked = false;
                                }                               
                        }
                        else if (item.type === "radio") {
                                radio = document.getElementsByName(name);
                                for (var i = 0; i < radio.length; i++) {
                                        if (radio[i].value === value) {
                                                radio[i].checked = true;
                                        }
                                }

                        }
                        else {
                                item.value = value;
                        }
                    }
                }
            } 
            else{
                    
                    var byteArray = new Uint8Array(evt.data);

                    var index = byteArray.indexOf(38);
                    var header = String.fromCharCode.apply(String,byteArray.slice(0, index));
                    
                    byteArray = byteArray.slice(index + 1);
                    
                    onStreamReceived(header, byteArray);
            }
        }
        
        /**
        * @brief private Event handler when websocket error
        */   
        var private_onError = function(evt)
        { 
            onError(evt);	
        }
	
}
