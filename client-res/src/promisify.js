
const promisify = (func) => {
    return (...funcParams) => {
        return new Promise((resolve, reject) => {
            if(funcParams.length == 0) {
                func((e,d) => {
                    if(e) {
                        reject(e);
                    } else {
                        resolve(d);
                    }
                });
            }
            if(funcParams.length == 1) {
                func(funcParams[0], (e,d) => {
                    if(e) {
                        reject(e);
                    } else {
                        resolve(d);
                    }
                });
            }
            if(funcParams.length == 2) {
                func(funcParams[0], funcParams[1], (e,d) => {
                    if(e) {
                        reject(e);
                    } else {
                        resolve(d);
                    }
                });
            }
            if(funcParams.length == 3) {
                func(funcParams[0], funcParams[1], funcParams[2], (e,d) => {
                    if(e) {
                        reject(e);
                    } else {
                        resolve(d);
                    }
                });
            }
            if(funcParams.length == 4) {
                func(funcParams[0], funcParams[1], funcParams[2], funcParams[3], (e,d) => {
                    if(e) {
                        reject(e);
                    } else {
                        resolve(d);
                    }
                });
            }
        });
    }
}

export {promisify};