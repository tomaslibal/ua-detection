import Pg

class BasicDao:
    objId = None
    def __init__(self, objId):
        self.objId = objId
    def get(self):
        pass
    def save(self):
        pass

class TableDao(BasicDao):
    def __init__(self, objId):
        super(TableDao, self).__init__(objId)
