 #!/usr/bin/python3


#################################################################################################################################################
#                                                    CLASSES CONTAINING ALL THE APP FUNCTIONS                                                                                                    #
#################################################################################################################################################


class DB:

    def __init__(self,Config):

        from math import floor
        from os import getcwd
        from os.path import join
        from json import loads, dumps, dump
        from datetime import timedelta, datetime, timezone 
        from pymongo import MongoClient , errors, ReturnDocument
        from urllib import parse
        from urllib.request import  urlopen 
        from bson.objectid import ObjectId  
       
      
        self.Config                         = Config
        self.getcwd                         = getcwd
        self.join                           = join 
        self.floor                      	= floor 
        self.loads                      	= loads
        self.dumps                      	= dumps
        self.dump                       	= dump  
        self.datetime                       = datetime
        self.ObjectId                       = ObjectId 
        self.server			                = Config.DB_SERVER
        self.port			                = Config.DB_PORT
        self.username                   	= parse.quote_plus(Config.DB_USERNAME)
        self.password                   	= parse.quote_plus(Config.DB_PASSWORD)
        self.remoteMongo                	= MongoClient
        self.ReturnDocument                 = ReturnDocument
        self.PyMongoError               	= errors.PyMongoError
        self.BulkWriteError             	= errors.BulkWriteError  
        self.tls                            = False # MUST SET TO TRUE IN PRODUCTION


    def __del__(self):
            # Delete class instance to free resources
            pass
 


    ####################
    # Weather Station Function #
    ####################


    
    def addUpdate(self,data):
        '''ADD A NEW STORAGE LOCATION TO COLLECTION'''
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = remotedb.ELET2415.weatherstation.insert_one(data)
        except Exception as e:
            msg = str(e)
            if "duplicate" not in msg:
                print("addUpdate error ",msg)
            return False
        else:                  
            return True
    

    #FUNCTION 2
    def getAllInRange(self,start, end):
        '''RETURNS A LIST OF OBJECTS. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            #result      = list(remotedb.ELET2415.climo.find({'timestamp': {'$gte': start, '$lte': end}}, {'_id': 0}).sort({'timestamp': 1}))
            result      = list(remotedb.ELET2415.weatherstation.find({'timestamp': {'$gte': start, '$lte': end}},{'_id': 0}).sort('timestamp', 1))
        except Exception as e:
            msg = str(e)
            print("getAllInRange error ",msg)            
        else:                  
            return result
    
    #FUNC 3

    def humidityMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{'$match': {'timestamp': {'$gte': start, '$lte': end}}}, {'$group': {'_id':0, 'Humidity': {'$push': '$$ROOT.Humidity'}}}, 
            {'$project': {'max': {'$max': '$Humidity'}, 'min': {'$min': '$Humidity'}, 'avg': {'$avg': '$Humidity'}, 'range': {'$subtract': [{'$max': '$Humidity'}, {'$min': '$Humidity'}]}}}]))

        except Exception as e:
            msg = str(e)
            print("humidityMMAR error ",msg)            
        else:                  
            return result


    def farenheitMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{'$match': {'timestamp': {'$gte': start, '$lte': end}}}, {'$group': {'_id':0, 'Farenheit': {'$push': '$$ROOT.Farenheit'}}}, 
            {'$project': {'max': {'$max': '$Farenheit'}, 'min': {'$min': '$Farenheit'}, 'avg': {'$avg': '$Farenheit'}, 'range': {'$subtract': [{'$max': '$Farenheit'}, {'$min': '$Farenheit'}]}}}]))

        except Exception as e:
            msg = str(e)
            print("farenheitMMAR error ",msg)            
        else:                  
            return result


    #FUNC 4
    def temperatureMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR TEMPERATURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            # start = int(start)
            # end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            #print(type(start))
            #print(type(end))
            #print(start)
            #print(end)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{ '$match': { 'timestamp': { '$gte': start, '$lte': end } } }, 
            { '$group': { '_id': 'Temperature', 'Temperature': { '$push': '$$ROOT.Temperature' } } }, { '$project': { 'max': { '$max': '$Temperature' }, 'min': { '$min': '$Temperature' }, 'avg': { '$avg': '$Temperature' }, 'range': { '$subtract': [ { '$max': '$Temperature' }, { '$min': '$Temperature' } ] } } } ]))
            #result = list(remotedb.ELET2415.climo.aggregate([ { '$match': {} } ]))
            print(result)
        except Exception as e:
            msg = str(e)
            print("temperatureMMAR error ",msg)            
        else:                 
            return result
        
    

    def HeatIndexMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{'$match': {'timestamp': {'$gte': start, '$lte': end}}}, {'$group': {'_id':0, 'Heat Index C': {'$push': '$$ROOT.Heat Index C'}}}, 
            {'$project': {'max': {'$max': '$Heat Index C'}, 'min': {'$min': '$Heat Index C'}, 'avg': {'$avg': '$Heat Index C'}, 'range': {'$subtract': [{'$max': '$Heat Index C'}, {'$min': '$Heat Index C'}]}}}]))

           
        except Exception as e:
            msg = str(e)
            print("HeatIndexMMAR error ",msg)            
        else:                  
            return result





    def pressureMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{'$match': {'timestamp': {'$gte': start, '$lte': end}}}, {'$group': {'_id':0, 'Pressure': {'$push': '$$ROOT.Pressure'}}},
             {'$project': {'max': {'$max': '$Pressure'}, 'min': {'$min': '$Pressure'}, 'avg': {'$avg': '$Pressure'}, 'range': {'$subtract': [{'$max': '$Pressure'}, {'$min': '$Pressure'}]}}}]))

        except Exception as e:
            msg = str(e)
            print("pressureMMAR error ",msg)            
        else:                  
            return result
    

    def altitudeMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{'$match': {'timestamp': {'$gte': start, '$lte': end}}}, {'$group': {'_id':0, 'Altitude': {'$push': '$$ROOT.Altitude'}}},
             {'$project': {'max': {'$max': '$Altitude'}, 'min': {'$min': '$Altitude'}, 'avg': {'$avg': '$Altitude'}, 'range': {'$subtract': [{'$max': '$Altitude'}, {'$min': '$Altitude'}]}}}]))

        except Exception as e:
            msg = str(e)
            print("altitudeMMAR error ",msg)            
        else:                  
            return result


    def soilmoistureMMAR(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = list(remotedb.ELET2415.weatherstation.aggregate([{'$match': {'timestamp': {'$gte': start, '$lte': end}}}, {'$group': {'_id':0, 'Soil Moisture': {'$push': '$$ROOT.Soil Moisture'}}},
             {'$project': {'max': {'$max': '$Soil Moisture'}, 'min': {'$min': '$Soil Moisture'}, 'avg': {'$avg': '$Soil Moisture'}, 'range': {'$subtract': [{'$max': '$Soil Moisture'}, {'$min': '$Soil Moisture'}]}}}]))

        except Exception as e:
            msg = str(e)
            print("soilmoistureMMAR error ",msg)            
        else:                  
            return result
    






    def frequencyDistro(self,variable,start, end):
        '''RETURNS THE FREQUENCY DISTROBUTION FOR A SPECIFIED VARIABLE WITHIN THE START AND END DATE RANGE'''
        try:
            start=int(start)
            end=int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weatherstation.aggregate([{
                            '$match': {
                            'timestamp': { '$gte': start, '$lte': end}
                            }
                        },
                     
                        {
                            '$bucket': {
                                'groupBy': f"${variable}",
                                'boundaries': list(range(101)),
                                'default': 'outliers',
                                'output': {
                                    'count': { '$sum': 1 }
                                }
                            }
                        }]))
        
        except Exception as e:
            msg = str(e)
            print("frequencyDistro error ",msg)            
        else:                  
            return result


def main():
    from config import Config
    from time import time, ctime, sleep
    from math import floor
    from datetime import datetime, timedelta
    one = DB(Config)
 
 
    start = time() 
    end = time()
    print(f"completed in: {end - start} seconds")
    
if __name__ == '__main__':
    main()


    
