#include <Arduino.h>
#include <MQTTClient.h>

typedef struct putContentNode{
	putContentNode *nextNode;
	
}PutContentNode;

boolean winikimLoop();
boolean winikimConnected();

//For Private Platform
boolean winikimConnect(const char clientId[], const char username[], const char password[]);

//For ThingsOfValue Platform
boolean winikimConnect();

boolean winikimSubscribe();
void winikimBegin(Client &net, const char *AeId,  const char *MqttSvr, const char *CseId, MQTTClientCallbackSimple cb);
void putContent(String containerName, String contentType, String data);
void createPutContentJson(const char cseId[], const char aeId[], const char containerName[], const char contentType[], const char data[]);
