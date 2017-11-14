import requests


URL = "http://call-flow-testing.appspot.com/"


class arduino_Callflow():
    def get_nodes(self):
        print(requests.get(URL + "nodes").json())

    def get_node(self, node):
        print(requests.get(URL + "nodes/{}".format(node)).json())

    def get_Actions_for_batch(self, **kwargs):
        for key in kwargs.items():
            if key == "batch":
                batch = key
                print(requests.get(URL + "batches/{}".format(batch)).json())

    def get_batches(self):
        print(requests.get(URL + "batches").json())

    def delete_batches(self, id):
        print(requests.delete(URL + "batches/{}".format(id)).json())

    def delete_node(self, node):
        print(requests.delete(URL + "nodes/{}".format(node)).json())

    def create_node(self, **kwargs):
        for key, value in kwargs.items():
            if key == "id" and value != "":
                response = requests.post(URL + "nodes/?", data=kwargs)
                if response.status_code == 200:
                    print("Node created successfully")
                    print(response.json())
                else:
                    print("Node creation failed")


    def create_batch(self, id):
        print(requests.post(URL + "batches/{}".format(id)).json())

    def create_action(self, **kwargs):
        for key, value in kwargs.items():
            if key.lower() == "phonenumber" and value != "":
                requests.post(URL + "actions", data=kwargs)

    def finish_node(self, node):
        response = requests.post(URL + "nodes/{}/finish".format(node))
        print(response.json())

    def start_node(self, node):
        response = requests.post(URL + "nodes/{}/start".format(node))
        print(response.json())

    def add_batch_to_node(self, **kwargs):

        for key in kwargs.items():
            if key == "node":
                node = key
                response = requests.post(URL + "nodes/{}".format(node),
                    data=kwargs)
                print(response.json())

    def arduino_help(self):
        print("Arduino_Help\n____________\n\n"
              "Create Node(id = node, description)\n"
              "Create Batch(id = batch)\n"
              "Create Action(BatchId, phoneNumber = B MSISDN, type = CALL, amount, duration, content, index)\n"
              "Finish Node(id = node)\n"
              "Start Node(id = node)\n"
              "Add Batch to Node(BatchId)")


Axel = arduino_Callflow()
arduino_Callflow.create_node(Axel, id="gamma")
