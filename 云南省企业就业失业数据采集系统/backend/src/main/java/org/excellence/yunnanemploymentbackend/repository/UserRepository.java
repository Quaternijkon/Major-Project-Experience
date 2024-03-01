package org.excellence.yunnanemploymentbackend.repository;

import org.excellence.yunnanemploymentbackend.entity.User;
import org.springframework.data.repository.CrudRepository;

public interface UserRepository extends CrudRepository<User, String> {
    
}
