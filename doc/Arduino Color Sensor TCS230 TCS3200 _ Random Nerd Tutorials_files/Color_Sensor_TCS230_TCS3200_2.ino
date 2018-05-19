if ( 'prettyPrint' in window ) {} else {
    document.write( '<script type="text/javascript" src="https://gist-it.appspot.com/assets/prettify/prettify.js"></script>' );
}


document.write( '<link rel="stylesheet" href="https://gist-it.appspot.com/assets/embed.css"/>' );


document.write( '<link rel="stylesheet" href="https://gist-it.appspot.com/assets/prettify/prettify.css"/>' );

document.write( '<div class="gist-it-gist">\n<div class="gist-file">\n    <div class="gist-data">\n        \n        <pre class="prettyprint">/*********\n  Rui Santos\n  Complete project details at http://randomnerdtutorials.com  \n*********/\n\n// TCS230 or TCS3200 pins wiring to Arduino\n#define S0 4\n#define S1 5\n#define S2 6\n#define S3 7\n#define sensorOut 8\n\n// Stores frequency read by the photodiodes\nint redFrequency = 0;\nint greenFrequency = 0;\nint blueFrequency = 0;\n\n// Stores the red. green and blue colors\nint redColor = 0;\nint greenColor = 0;\nint blueColor = 0;\n\nvoid setup() {\n  // Setting the outputs\n  pinMode(S0, OUTPUT);\n  pinMode(S1, OUTPUT);\n  pinMode(S2, OUTPUT);\n  pinMode(S3, OUTPUT);\n  \n  // Setting the sensorOut as an input\n  pinMode(sensorOut, INPUT);\n  \n  // Setting frequency scaling to 20%\n  digitalWrite(S0,HIGH);\n  digitalWrite(S1,LOW);\n  \n  // Begins serial communication\n  Serial.begin(9600);\n}\n\nvoid loop() {\n  // Setting RED (R) filtered photodiodes to be read\n  digitalWrite(S2,LOW);\n  digitalWrite(S3,LOW);\n  \n  // Reading the output frequency\n  redFrequency = pulseIn(sensorOut, LOW);\n  // Remaping the value of the RED (R) frequency from 0 to 255\n  // You must replace with your own values. Here\'s an example: \n  // redColor = map(redFrequency, 70, 120, 255,0);\n  redColor = map(redFrequency, XX, XX, 255,0);\n  \n  // Printing the RED (R) value\n  Serial.print("R = ");\n  Serial.print(redColor);\n  delay(100);\n  \n  // Setting GREEN (G) filtered photodiodes to be read\n  digitalWrite(S2,HIGH);\n  digitalWrite(S3,HIGH);\n  \n  // Reading the output frequency\n  greenFrequency = pulseIn(sensorOut, LOW);\n  // Remaping the value of the GREEN (G) frequency from 0 to 255\n  // You must replace with your own values. Here\'s an example: \n  // greenColor = map(greenFrequency, 100, 199, 255, 0);\n  greenColor = map(greenFrequency, XX, XX, 255, 0);\n  \n  // Printing the GREEN (G) value  \n  Serial.print(" G = ");\n  Serial.print(greenColor);\n  delay(100);\n \n  // Setting BLUE (B) filtered photodiodes to be read\n  digitalWrite(S2,LOW);\n  digitalWrite(S3,HIGH);\n  \n  // Reading the output frequency\n  blueFrequency = pulseIn(sensorOut, LOW);\n  // Remaping the value of the BLUE (B) frequency from 0 to 255\n  // You must replace with your own values. Here\'s an example: \n  // blueColor = map(blueFrequency, 38, 84, 255, 0);\n  blueColor = map(blueFrequency, XX, XX, 255, 0);\n  \n  // Printing the BLUE (B) value \n  Serial.print(" B = ");\n  Serial.print(blueColor);\n  delay(100);\n\n  // Checks the current detected color and prints\n  // a message in the serial monitor\n  if(redColor &gt; greenColor &amp;&amp; redColor &gt; blueColor){\n      Serial.println(" - RED detected!");\n  }\n  if(greenColor &gt; redColor &amp;&amp; greenColor &gt; blueColor){\n    Serial.println(" - GREEN detected!");\n  }\n  if(blueColor &gt; redColor &amp;&amp; blueColor &gt; greenColor){\n    Serial.println(" - BLUE detected!");\n  }\n}\n</pre>\n        \n    </div>\n    \n    <div class="gist-meta">\n        \n        <span style="float: right; color: #369;"><a href="https://github.com/RuiSantosdotme/Random-Nerd-Tutorials/raw/master/Projects/color/Color_Sensor_TCS230_TCS3200_2.ino">view raw</a></span>\n        <span style="float: right; margin-right: 8px;">\n            <a style="color: rgb(102, 102, 102);" href="https://github.com/RuiSantosdotme/Random-Nerd-Tutorials/blob/master/Projects/color/Color_Sensor_TCS230_TCS3200_2.ino">Projects/color/Color_Sensor_TCS230_TCS3200_2.ino</a></span>\n            <!-- Generated by: https://gist-it.appspot.com -->\n    </div>\n    \n</div>\n</div>' );

document.write( '<script type="text/javascript">prettyPrint();</script>' );