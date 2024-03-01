package org.excellence.yunnanemploymentbackend.repository;

import org.excellence.yunnanemploymentbackend.entity.Enterprise;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

public interface EnterpriseRepository extends CrudRepository<Enterprise, String> {
    List<Enterprise> findEnterprisesByIsRegisteredIsTrue();

    List<Enterprise> findEnterprisesByIsRegisteredIsTrueAndLocatedCityIs(String locatedCity);
}
