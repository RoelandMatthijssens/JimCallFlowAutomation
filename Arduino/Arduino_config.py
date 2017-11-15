import requests
import sys
import pprint


URL = 'http://call-flow-testing.appspot.com/'

class action():
    def __init__(self, **kwargs):
        self.BatchId = kwargs.get('BatchId', None)
        self.phoneNumber = kwargs.get('phoneNumber', None)
        self.type = kwargs.get('type', None)
        self.amount = kwargs.get('amount', None)
        self.duration = kwargs.get('duration', None)
        self.content = kwargs.get('content', None)
        self.index = kwargs.get('index', None)

    def list(self):
        print('listeddd')

    def create(self):
        assert self.phoneNumber
        assert self.BatchId
        assert self.index
        pprint.pprint(requests.post(URL + 'actions', data=self.__dict__).json())


class batch():
    def __init__(self, **kwargs):
        self.id = kwargs.get('id', None)

    def list(self):
        pprint.pprint(requests.get(URL + 'batches').json())

    def create(self):
        assert self.id
        pprint.pprint(requests.post(URL + 'batches', data=self.__dict__).json())


class node():
    def __init__(self, **kwargs):
        pass

    def list(self):
        pass

    def create(self):
        pass



<<<<<<< HEAD
=======
URL = 'http://call-flow-testing.appspot.com/'
>>>>>>> d8e046fa16067ab88461fe61f8145ffc34667073


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

<<<<<<< HEAD
    def create_action(self, action):
            print(requests.post(URL + 'actions', ).json())
=======
    def create_action(self, **kwargs):
        if kwargs['phoneNumber']:
            print(requests.post(URL + 'actions', data=kwargs).json())
>>>>>>> d8e046fa16067ab88461fe61f8145ffc34667073

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


<<<<<<< HEAD
def help():
    print('Arduino_Help\n____________\n\n'
          'Create Node(id = node, description)\n'
          'Create Batch(id = batch)\n'
          'Create Action(BatchId, phoneNumber = B MSISDN, type = CALL, amount, duration, content, index)\n'
          'Finish Node(id = node)\n'
          'Start Node(id = node)\n'
          'Add Batch to Node(BatchId)')


def split_argument_list(argument_list):
    d = {}
    for i in argument_list:
        k, v = i.split("=")
        d[k] = v
    return d

def callMethod(o, name):
    getattr(o, name)()

def parse_input(command_line_args):
    command = command_line_args[1]
    model_name = command_line_args[0]
    clas = eval(model_name)
    argument_dict = split_argument_list(command_line_args[2:])
    instance = clas(**argument_dict)
    callMethod(instance, command)

if __name__=='__main__':
    args = sys.argv[1:]  # create action name=nekfwlfw ...
    if len(args) == 0:
        help()
        exit(1)
    parse_input(args)
=======
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
>>>>>>> d8e046fa16067ab88461fe61f8145ffc34667073
