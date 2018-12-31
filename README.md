# ParticlePhotonPulseCounter

This is a pulse counter made on the particle photon which senses a pulse on pin D5 and counts it. This information is then published as a json webhook to the particle cloud. I have the particle cloud webhook set up to transfer the data to my shared hosting website's MySQL database. I am positng this here just in case any pieces may help someone else trying to do the same thing. 

## Getting Started
You can use the particle webIDE to flash this software to your photon

### Attributes
This would not have been possible without the help of @Rftop on the particle community forums who helped me through the whole process of troubleshooting and rewrote my base for me. I really appreciate all the help.

###Instructions
The IDE code can be copied into the webIDE at build.particle.io and flashed to your device. The JOSN post integration can be copied into the JSON form for the webhook integration that you use the particle console to set up (click on 'integrations') and the PHP code is going to be put into the file in public_html folder on your website where the http url you put in your webhook integration actually lands. Make sure to make a directory file with your database name and log in information in that folder as well with restricted credentials so that you php has the log in information to post data.
