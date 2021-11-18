import csv, requests #, googlemaps

centerNames = []
transplantCenters = []

with open('transplantCenterData.csv', 'r') as centers:
    reader = csv.reader(centers)

    header = next(reader)

    for row in reader:
        centerNames.append(row[0])
        transplantCenters.append(row[1])

#print(transplantCenters)
#print(centerNames)

OPONames = []
OPOs = []

with open('OPODirectory.csv', 'r') as centers:
    reader = csv.reader(centers)

    header = next(reader)

    for row in reader:
        OPONames.append(row[0])
        OPOs.append(row[1])

#print(OPOs)
#print(OPONames)

#key = 'AIzaSyAG1Vf3D4umti_eN9yyaUyl6KgJj4V1BUM'
#client = googlemaps.Client(key)

#matrix = client.distance_matrix(OPOs, transplantCenters, mode="driving", units="imperial")

with open('distances.csv', 'w') as distances:
    writer = csv.writer(distances)
    writer.writerow([''] + centerNames)

    i = 0
    for name in OPONames:
        elements = []
        
        #for element in matrix["rows"][i]["elements"]:
        #    elements.append(element["distance"]["value"])

        for center in transplantCenters:
            params = {'locations' : [OPOs[i], center]}
            response = requests.post('http://www.mapquestapi.com/directions/v2/routematrix?key=TVG4TtcxmnCXjBC90J9n9Ez3OrjH88kp', json = params)
            print(response.json())
            print(params)

            if not response.json()['info']['messages']:
                elements.append(response.json()['distance'][1])
            else:
                elements.append('Too far')

        i = i + 1


        writer.writerow([name] + elements)

#print(matrix)

