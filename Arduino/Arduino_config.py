import requests


URL = 'http://call-flow-testing.appspot.com/'


class arduino_Callflow():
    def get_nodes(self):
        print(requests.get(URL + 'nodes').json())

    def get_node(self, node):
        print(requests.get(URL + f'nodes/{node}').json())

    def get_actions_for_batch(self, **kwargs):
        if kwargs['id']:
            batch = kwargs.get('id')
            print(requests.get(URL + f'batches/{batch}').json())

    def get_batches(self):
        print(requests.get(URL + 'batches').json())

    def delete_batches(self, id):
        print(requests.delete(URL + f'batches/{id}').json())

    def delete_node(self, node):
        print(requests.delete(URL + f'nodes/{node}').json())

    def create_node(self, **kwargs):
        if  not kwargs['id']:
            print('Node is not created, please add id as an argument')
            return
        print(requests.post(URL + 'nodes/?', data = kwargs).json())

    def create_batch(self, batchid):
        print(requests.post(URL + 'batches', data = {'id':batchid}).json())

    def create_action(self, **kwargs):
        if kwargs['phoneNumber']:
            print(requests.post(URL + 'actions', data=kwargs).json())

    def finish_node(self, node):
        response = requests.post(URL + f'nodes/{node}/finish')
        print(response.json())

    def start_node(self, node):
        response = requests.post(URL + f'nodes/{node}/start')
        print(response.json())

    def add_batch_to_node(self, **kwargs):
        if kwargs['node']:
            node = kwargs.get('node')
            print(requests.post(URL + f"nodes/{node}", data=kwargs))


    def arduino_help(self):
        print('Arduino_Help\n____________\n\n'
              'Create Node(id = node, description)\n'
              'Create Batch(id = batch)\n'
              'Create Action(BatchId, phoneNumber = B MSISDN, type = CALL, amount, duration, content, index)\n'
              'Finish Node(id = node)\n'
              'Start Node(id = node)\n'
              'Add Batch to Node(BatchId)')


    Axel = arduino_Callflow()
arduino_Callflow.get_batches(Axel)