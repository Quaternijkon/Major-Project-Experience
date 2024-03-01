package org.excellence.yunnanemploymentbackend.controller;

import org.excellence.yunnanemploymentbackend.entity.EnterpriseInfo;
import org.excellence.yunnanemploymentbackend.entity.ReportedData;
import org.excellence.yunnanemploymentbackend.entity.Response;
import org.excellence.yunnanemploymentbackend.service.EnterpriseService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.bind.annotation.CrossOrigin;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping(path = "enterprise")
@CrossOrigin("*")
public class EnterpriseController {
    @Autowired
    private EnterpriseService enterpriseService;

    /**
     * 企业备案
     * @param enterpriseInfo 备案信息
     * @return 操作是否成功
     */

    @CrossOrigin("*")
    @PostMapping(path = "update-enterprise-info/{province-user-id}/{enterprise-user-id}")
    public @ResponseBody Response<Boolean> updateEnterpriseInfo(
            @RequestBody EnterpriseInfo enterpriseInfo,
            @PathVariable("enterprise-user-id") String enterpriseUserId,
            @PathVariable("province-user-id") String provinceUserId) {
        try {
            return new Response<>(true, "success", enterpriseService.addNewInfo(provinceUserId, enterpriseUserId, enterpriseInfo));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 企业上报信息
     * @param provinceUserId 企业所在省的userId
     * @param enterpriseUserId 企业的userId
     * @param reportedData 上报信息
     * @return 操作是否成功
     */

    @CrossOrigin("*")
    @PostMapping(path = "report-data/{province-user-id}/{enterprise-user-id}")
    public @ResponseBody Response<Boolean> reportData(
            @PathVariable("enterprise-user-id") String enterpriseUserId,
            @PathVariable("province-user-id") String provinceUserId,
            @RequestBody ReportedData reportedData) {
        try {
            return new Response<>(true, "success", enterpriseService.addNewInfo(provinceUserId, enterpriseUserId, reportedData));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 获取企业所有历史上报信息
     * @param enterpriseUserId 企业的userId
     * @return 上报信息列表
     */
    @CrossOrigin("*")
    @GetMapping(path = "get-all-reported-data/{enterprise-user-id}")
    public @ResponseBody Response<List<ReportedData>> getAllReportedData(
            @PathVariable("enterprise-user-id") String enterpriseUserId) {
        try {
            return new Response<>(true, "success", enterpriseService.getAllReportedData(enterpriseUserId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), new ArrayList<>());
        }
    }
}
