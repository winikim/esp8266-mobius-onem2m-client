#include <winikim.h>

const char *aeId;
const char *mqttSvr;
const char *cseId;


char mqttPwd[38];
char subscribeTopic[256];
char publishTopic[256];
char putContentJson[1024];

MQTTClient client(1024);
int qos = 1;

int nodeCount = 0;
PutContentNode *head = NULL;
PutContentNode *tail = NULL;

boolean winikimLoop() { return client.loop(); }
boolean winikimConnected() { return client.connected(); }
boolean winikimConnect(){
	return client.connect(aeId, aeId, mqttPwd);
}

boolean winikimConnect(const char clientId[], const char username[], const char password[]){ return client.connect(clientId, username, password); }

boolean winikimSubscribe(){
	boolean unsub = client.unsubscribe(subscribeTopic);
	boolean sub = client.subscribe(subscribeTopic);
	return unsub && sub;
}

void winikimBegin(Client &net, const char *AeId, const char *MqttSvr, const char *CseId, MQTTClientCallbackSimple cb){
	aeId = AeId;
	mqttSvr = MqttSvr;
	cseId = CseId;
	sprintf(subscribeTopic, "/oneM2M/req/+/%s/json", aeId);
 	sprintf(publishTopic, "/oneM2M/req/%s/%s/json", aeId, cseId);
	client.begin(mqttSvr, net);
	client.onMessage(cb);
}

void putContent(String containerName, String contentType, String data) {
  createPutContentJson(cseId, aeId, containerName.c_str(), contentType.c_str(), data.c_str());
  client.publish(publishTopic, putContentJson, true, qos);
}

void createPutContentJson(const char cseId[], const char aeId[], const char containerName[], const char contentType[], const char data[]) {
	sprintf(putContentJson, "{\"m2m:rqp\":{\"op\":1,\"to\":\"%s/base/%s/%s\",\"fr\":\"/%s\",\"rqi\":\"rqi-%s-%lu\",\"ty\":4,\"pc\":{\"m2m:cin\":{\"cnf\":\"%s:0\",\"con\":\"%s\"}}}}", cseId, aeId, containerName, aeId, aeId, millis(), contentType, data);
}


/*
void enqueNode(String containerName, String contentType, String data){
	
}
*/
