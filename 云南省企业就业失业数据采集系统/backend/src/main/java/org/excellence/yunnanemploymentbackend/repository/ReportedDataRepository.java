package org.excellence.yunnanemploymentbackend.repository;

import org.excellence.yunnanemploymentbackend.entity.ReportedData;
import org.springframework.data.repository.CrudRepository;
import java.util.List;

public interface ReportedDataRepository extends CrudRepository<ReportedData, Integer> {
    List<ReportedData> findByEnterpriseUserId(String enterpriseUserId);
}
