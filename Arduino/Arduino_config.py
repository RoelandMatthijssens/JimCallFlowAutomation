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
        pprint.pprint(requests.get(URL + 'actions').json())

    def create(self):
        assert self.phoneNumber
        assert self.BatchId
        assert self.index
        pprint.pprint(requests.post(URL + 'actions', data=self.__dict__).json())

class batch():
    def __init__(self, **kwargs):
        self.id = kwargs.get('id', None)
        self.node = kwargs.get('node', None)

    def list(self):
        pprint.pprint(requests.get(URL + 'batches').json())

    def create(self):
        assert self.id
        pprint.pprint(requests.post(URL + 'batches', data=self.__dict__).json())

    def delete(self):
        assert self.id
        pprint.pprint(requests.delete(URL + f'batches/{self.id}').json())

    def listActions(self):
        assert self.id
        pprint.pprint(requests.get(URL + f'batches/{self.id}').json())


class node():
    def __init__(self, **kwargs):
        self.node = kwargs.get('node', None)
        self.id = kwargs.get('id', None)
        self.description = kwargs.get('description', None)
        self.BatchId = kwargs.get('BatchId', None)

    def list(self):
        pprint.pprint(requests.get(URL + 'nodes').json())

    def state(self):
        assert self.node
        pprint.pprint(requests.post(URL + f'nodes/{self.node}').json())

    def create(self):
        assert self.id
        pprint.pprint(requests.post(URL + 'nodes/?', data=self.__dict__).json())

    def delete(self):
        assert self.node
        pprint.pprint(requests.delete(URL + f'nodes/{self.node}').json())

    def linkBatches(self):
        assert self.BatchId
        assert self.node
        pprint.pprint(requests.post(URL + f'nodes/{self.node}', data=self.__dict__).json())

    def getBatches(self):
        assert self.node
        pprint.pprint(requests.post(URL + f'nodes/{self.node}').json())

    def start(self):
        assert self.node
        pprint.pprint(requests.post(URL + f'nodes/{self.node}/start').json())

    def finish(self):
        assert self.node
        pprint.pprint(requests.post(URL + f'nodes/{self.node}/finish').json())

def help():
    print('____________\nArduino_Help\n____________\n\n')
    print('Command to run: python3 Arduino_config.py Model Command Attribute1 Attribute2 ...\n\n')
    print('Model: action\n\tCommand: list\n\tCommand: create\n\t\t\tBatchId\n\t\t\tphoneNumber'
          '\n\t\t\ttype (SMS or CALL)\n\t\t\tamount\n\t\t\tcontent (in case of SMS: what to be send)'
          '\n\t\t\tindex (order of the action in batch starting at 0)')
    print('Model: batch\n\tCommand: list\n\tCommand: create\n\t\t\tid (the name of the batch)'
          '\n\tCommand: delete\n\t\t\tid (the name of the batch)\n\tCommand: listActions'
          '\n\t\t\tid (the name of the batch)')
    print('Model: node\n\tCommand: list\n\tCommand: state\n\t\t\t node\n\tCommand: create\n\t\t\t id'
          '\n\tCommand: delete\n\t\t\tnode\n\tCommand: linkBatches\n\t\t\tBatchId\n\t\t\tnode'
          '\n\tCommand: getBatches\n\t\t\tnode\n\tCommand: start\n\t\t\tnode'
          '\n\tCommand: finish\n\t\t\tnode')

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

