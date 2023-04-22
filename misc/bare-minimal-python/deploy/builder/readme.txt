# to build (cd to directory with dockerfile)
docker build --no-cache -t pybaremin_builder .
# to copy built file
docker run -it -v $(pwd)/:/transfer pybaremin_builder /bin/bash -c "cp /build/cpython-3.12.0a6/build.tar.gz /transfer/build.tar.gz"


# error is ok here (in Dockerfile), build goes successful
# RUN (./configure && make -j); exit 0


# to run (debugging)
PYTHONPATH=./Lib ./python

# !! COPY build.tar.gz to runner !!


# to wipe everything (from https://gist.github.com/beeman/aca41f3ebd2bf5efbd9d7fef09eac54d)
docker stop $(docker ps -qa); docker rm $(docker ps -qa); docker rmi -f $(docker images -qa); docker volume rm $(docker volume ls -q); docker network rm $(docker network ls -q)

