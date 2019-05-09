def load(filename):
    config = dict()
    try:
        with open(filename, 'r') as fd:
                for line in fd:
                    key, value = line.split()
                    print(key)
                    print(value)
                    self.config[key] = value
    except:
        pass
    return config