package org.excellence.yunnanemploymentbackend.repository;

import org.excellence.yunnanemploymentbackend.entity.EnterpriseInfo;
import org.springframework.data.repository.CrudRepository;

public interface EnterpriseInfoRepository extends CrudRepository<EnterpriseInfo, Integer> {
    EnterpriseInfo findEnterpriseInfoByEnterpriseUserId(String userId);
}
